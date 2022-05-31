#include "client/tatp/TatpClient.hpp"
#include "txbench/pq/ResultSet.hpp"
#include <charconv>
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
TatpClient::TatpClient(const ClientConfig& clientConfig, const TatpConfig& tatpConfig, unsigned clientIndex)
   : Client(clientConfig), tatpConfig(tatpConfig), generator(42 + clientIndex), subscriberCount(0)
// Constructor
{
}
//---------------------------------------------------------------------------
void TatpClient::performWork()
// Perform work
{
   auto pipelineDepth = clientConfig.getPipelineDepth();

   connection.enterPipelineMode();

   vector<pq::AsyncCall> receivingPipeline;
   receivingPipeline.reserve(pipelineDepth);
   vector<pq::AsyncCall> sendingPipeline;
   sendingPipeline.reserve(pipelineDepth);

   while (!done.load(memory_order_acquire)) {
      // Enqueue some transactions
      for (unsigned i = 0; i < pipelineDepth; ++i) {
         switch (generator.pickTransactionType()) {
            case tatp::TransactionGenerator::TransactionType::GetSubscriberData:
               sendingPipeline.push_back(getSubscriberData.bind(subscriberCount));
               break;
            case tatp::TransactionGenerator::TransactionType::GetNewDestination:
               sendingPipeline.push_back(getNewDestination.bind(subscriberCount));
               break;
            case tatp::TransactionGenerator::TransactionType::GetAccessData:
               sendingPipeline.push_back(getAccessData.bind(subscriberCount));
               break;
            case tatp::TransactionGenerator::TransactionType::UpdateSubscriberData:
               sendingPipeline.push_back(updateSubscriberData.bind(subscriberCount));
               break;
            case tatp::TransactionGenerator::TransactionType::UpdateLocation:
               sendingPipeline.push_back(updateLocation.bind(subscriberCount));
               break;
            case tatp::TransactionGenerator::TransactionType::InsertCallForwarding:
               sendingPipeline.push_back(insertCallForwarding.bind(subscriberCount));
               break;
            case tatp::TransactionGenerator::TransactionType::DeleteCallForwarding:
               sendingPipeline.push_back(deleteCallForwarding.bind(subscriberCount));
               break;
         }

         if (auto res = sendingPipeline.back().send(); !res) {
            cerr << "send(): " << res.getMessage() << endl;
            return;
         }

         submitted.fetch_add(1, memory_order_acq_rel);
      }

      // Request that the server flushes results
      if (auto res = connection.enqueueFlushRequest(); !res) {
         cerr << "enqueueFlushRequest(): " << res.getMessage() << endl;
         return;
      }

      // And submit to the server
      if (auto res = connection.flush(); !res) {
         cerr << "flush(): " << res.getMessage() << endl;
         return;
      }

      // Process results from the previous batch (if any)
      if (!receivingPipeline.empty()) {
         for (unsigned i = 0; i < pipelineDepth; ++i) {
            auto result = receivingPipeline[i].receiveQuery();
            if (auto res = result.getStatus(); !res) {
               cerr << "receive(): " << res.getMessage() << endl;
               return;
            }

            processed.fetch_add(1, memory_order_acq_rel);
         }
         receivingPipeline.clear();
      }

      swap(sendingPipeline, receivingPipeline);
   }

   // Process results from the previous batch (if any)
   if (!receivingPipeline.empty()) {
      for (unsigned i = 0; i < pipelineDepth; ++i) {
         auto result = receivingPipeline[i].receiveQuery();
         if (auto res = result.getStatus(); !res) {
            cerr << "receive(): " << res.getMessage() << endl;
            return;
         }

         processed.fetch_add(1, memory_order_acq_rel);
      }
      receivingPipeline.clear();
   }

   connection.sync();
   connection.exitPipelineMode();
}
//---------------------------------------------------------------------------
Status TatpClient::initialize()
// Initialize the client
{
   if (auto res = Client::initialize(); !res)
      return res;

   {
      // Retrieve the number of warehouses
      auto result = connection.executeQuery("SELECT COUNT(*) FROM subscriber");
      if (auto res = result.getStatus(); !res)
         return res;

      auto field = result.get(0, 0);
      auto [ptr, ec] = from_chars(field.data(), field.data() + field.size(), subscriberCount);
      if (ptr != (field.data() + field.size()))
         return Status::Error("unable to parse subscriber count");
   }

   if (auto res = getSubscriberData.prepare(connection, "CALL get_subscriber_data($1::INTEGER)"); !res)
      return res;
   if (auto res = getNewDestination.prepare(connection, "CALL get_new_destination($1::INTEGER)"); !res)
      return res;
   if (auto res = getAccessData.prepare(connection, "CALL get_access_data($1::INTEGER)"); !res)
      return res;
   if (auto res = updateSubscriberData.prepare(connection, "CALL update_subscriber_data($1::INTEGER)"); !res)
      return res;
   if (auto res = updateLocation.prepare(connection, "CALL update_location($1::INTEGER)"); !res)
      return res;
   if (auto res = insertCallForwarding.prepare(connection, "CALL insert_call_forwarding($1::INTEGER)"); !res)
      return res;
   if (auto res = deleteCallForwarding.prepare(connection, "CALL delete_call_forwarding($1::INTEGER)"); !res)
      return res;

   return Status::Success;
}
//---------------------------------------------------------------------------
void TatpClient::start()
// Start processing
{
   done.store(false, memory_order_release);
   thread = ::thread([this]() { performWork(); });
}
//---------------------------------------------------------------------------
void TatpClient::stop()
// Stop processing
{
   done.store(true, memory_order_release);
}
//---------------------------------------------------------------------------
void TatpClient::join()
// Join the client
{
   thread.join();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
