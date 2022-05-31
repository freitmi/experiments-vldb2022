#include "TpccClient.hpp"
#include "txbench/pq/ResultSet.hpp"
#include <charconv>
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
TpccClient::TpccClient(const ClientConfig& clientConfig, const TpccConfig& tpccConfig, unsigned homeWarehouseId)
   : Client(clientConfig), tpccConfig(tpccConfig), generator(42 + homeWarehouseId), homeWarehouseId(homeWarehouseId), warehouseCount(0)
// Constructor
{
}
//---------------------------------------------------------------------------
void TpccClient::performWork()
// Perform work
{
   auto pipelineDepth = clientConfig.getPipelineDepth();

   bool generateRemoteAccesses = tpccConfig.shouldGenerateRemoteAccesses();
   bool generateUserRollbacks = tpccConfig.shouldGenerateUserRollbacks();

   connection.enterPipelineMode();

   vector<pq::AsyncCall> receivingPipeline;
   receivingPipeline.reserve(pipelineDepth);
   vector<pq::AsyncCall> sendingPipeline;
   sendingPipeline.reserve(pipelineDepth);

   while (!done.load(memory_order_acquire)) {
      // Enqueue some transactions
      for (unsigned i = 0; i < pipelineDepth; ++i) {
         switch (generator.pickTransactionType()) {
            case tpcc::TransactionGenerator::TransactionType::Delivery:
               sendingPipeline.push_back(delivery.bind(homeWarehouseId));
               break;
            case tpcc::TransactionGenerator::TransactionType::NewOrder:
               sendingPipeline.push_back(newOrder.bind(homeWarehouseId, warehouseCount, generateRemoteAccesses, generateUserRollbacks));
               break;
            case tpcc::TransactionGenerator::TransactionType::OrderStatus:
               sendingPipeline.push_back(orderStatus.bind(homeWarehouseId));
               break;
            case tpcc::TransactionGenerator::TransactionType::Payment:
               sendingPipeline.push_back(payment.bind(homeWarehouseId, warehouseCount, generateRemoteAccesses));
               break;
            case tpcc::TransactionGenerator::TransactionType::StockLevel:
               sendingPipeline.push_back(stockLevel.bind(homeWarehouseId));
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
            auto result = receivingPipeline[i].receiveCommand();
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
         auto result = receivingPipeline[i].receiveCommand();
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
Status TpccClient::initialize()
// Initialize the client
{
   if (auto res = Client::initialize(); !res)
      return res;

   {
      // Retrieve the number of warehouses
      auto result = connection.executeQuery("SELECT COUNT(*) FROM warehouse");
      if (auto res = result.getStatus(); !res)
         return res;

      auto field = result.get(0, 0);
      auto [ptr, ec] = from_chars(field.data(), field.data() + field.size(), warehouseCount);
      if (ptr != (field.data() + field.size()))
         return Status::Error("unable to parse warehouse count");
      if (homeWarehouseId > warehouseCount)
         return Status::Error("invalid home warehouse id");
   }

   if (auto res = delivery.prepare(connection, "CALL delivery($1::INTEGER)"); !res)
      return res;
   if (auto res = newOrder.prepare(connection, "CALL newOrder($1::INTEGER, $2::INTEGER, $3::INTEGER, $4::INTEGER)"); !res)
      return res;
   if (auto res = orderStatus.prepare(connection, "CALL orderStatus($1::INTEGER)"); !res)
      return res;
   if (auto res = payment.prepare(connection, "CALL payment($1::INTEGER, $2::INTEGER, $3::INTEGER)"); !res)
      return res;
   if (auto res = stockLevel.prepare(connection, "CALL stockLevel($1::INTEGER)"); !res)
      return res;

   return Status::Success;
}
//---------------------------------------------------------------------------
void TpccClient::start()
// Start processing
{
   done.store(false, memory_order_release);
   thread = ::thread([this]() { performWork(); });
}
//---------------------------------------------------------------------------
void TpccClient::stop()
// Stop processing
{
   done.store(true, memory_order_release);
}
//---------------------------------------------------------------------------
void TpccClient::join()
// Join the client
{
   thread.join();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
