#ifndef H_tools_postgres_client_TpccClient
#define H_tools_postgres_client_TpccClient
//---------------------------------------------------------------------------
#include "client/Client.hpp"
#include "client/tpcc/TpccConfig.hpp"
#include "txbench/pq/PreparedStatement.hpp"
#include "txbench/tpcc/TransactionGenerator.hpp"
#include <atomic>
#include <thread>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// TPCC client
class TpccClient : public Client {
   private:
   /// The TPCC configuration
   const TpccConfig& tpccConfig;

   /// The processing thread
   std::thread thread;
   /// Whether we are done
   std::atomic<bool> done;

   /// The transaction generator
   tpcc::TransactionGenerator generator;
   /// The home warehouse id
   unsigned homeWarehouseId;
   /// The warehouse count
   unsigned warehouseCount;

   /// The delivery statement
   pq::PreparedStatement delivery;
   /// The new order statement
   pq::PreparedStatement newOrder;
   /// The order status statement
   pq::PreparedStatement orderStatus;
   /// The payment statement
   pq::PreparedStatement payment;
   /// The stock level statement
   pq::PreparedStatement stockLevel;

   /// Perform work
   void performWork();

   public:
   /// Constructor
   explicit TpccClient(const ClientConfig& clientConfig, const TpccConfig& tpccConfig, unsigned homeWarehouseId);

   /// Initialize the client
   Status initialize() override;

   /// Start processing
   void start() override;
   /// Stop processing
   void stop() override;
   /// Join the client
   void join() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif