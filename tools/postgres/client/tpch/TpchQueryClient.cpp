#include "client/tpch/TpchQueryClient.hpp"
#include "txbench/pq/ResultSet.hpp"
#include <fstream>
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
static Status readScript(const string& filename, string& target)
// Read a query script from a file
{
   ifstream t(filename);
   if (!t.is_open())
      return Status::Error("unable to open \"" + filename + "\"");

   target = string((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
   return Status::Success;
}
//---------------------------------------------------------------------------
TpchQueryClient::TpchQueryClient(const ClientConfig& clientConfig, const TpchConfig& tpchConfig)
   : Client(clientConfig), tpchConfig(tpchConfig)
// Constructor
{
}
//---------------------------------------------------------------------------
void TpchQueryClient::performWork()
// Perform work
{
   auto pipelineDepth = 22;

   connection.enterPipelineMode();

   vector<pq::AsyncCall> receivingPipeline;
   receivingPipeline.reserve(pipelineDepth);
   vector<pq::AsyncCall> sendingPipeline;
   sendingPipeline.reserve(pipelineDepth);

   while (!done.load(memory_order_acquire)) {
      // Enqueue some transactions
      for (unsigned i = 0; i < pipelineDepth; ++i) {
         sendingPipeline.push_back(statements[i].bind());

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
Status TpchQueryClient::initialize()
// Initialize the client
{
   if (auto res = Client::initialize(); !res)
      return res;

   statements.resize(22);
   for (unsigned i = 0; i < 22; ++i) {
      string query;
      if (auto res = readScript("/scripts/queries/" + to_string(i + 1) + ".sql", query); !res)
         return res;
      if (auto res = statements[i].prepare(connection, query); !res)
         return res;
   }

   return Status::Success;
}
//---------------------------------------------------------------------------
void TpchQueryClient::start()
// Start processing
{
   done.store(false, memory_order_release);
   thread = ::thread([this]() { performWork(); });
}
//---------------------------------------------------------------------------
void TpchQueryClient::stop()
// Stop processing
{
   done.store(true, memory_order_release);
}
//---------------------------------------------------------------------------
void TpchQueryClient::join()
// Join the client
{
   thread.join();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------