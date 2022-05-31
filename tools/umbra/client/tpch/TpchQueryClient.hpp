#ifndef H_tools_umbra_client_tpch_TpchQueryClient
#define H_tools_umbra_client_tpch_TpchQueryClient
//---------------------------------------------------------------------------
#include "client/Client.hpp"
#include "client/tpch/TpchConfig.hpp"
#include "txbench/pq/PreparedStatement.hpp"
#include <thread>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// The query client for TPCH
class TpchQueryClient : public Client {
   private:
   /// The TPCH config
   const TpchConfig& tpchConfig;

   /// The processing thread
   std::thread thread;
   /// Whether we are done
   std::atomic<bool> done;

   /// The query statements
   std::vector<pq::PreparedStatement> statements;

   /// Perform work
   void performWork();

   public:
   /// Constructor
   explicit TpchQueryClient(const ClientConfig& clientConfig, const TpchConfig& tpchConfig);

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