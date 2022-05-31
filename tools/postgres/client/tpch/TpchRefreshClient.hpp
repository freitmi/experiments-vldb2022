#ifndef H_tools_postgres_client_tpch_TpchRefreshClient
#define H_tools_postgres_client_tpch_TpchRefreshClient
//---------------------------------------------------------------------------
#include "client/Client.hpp"
#include "client/tpch/TpchConfig.hpp"
#include <thread>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// The refresh client for TPCH
class TpchRefreshClient : public Client {
   private:
   /// The TPCH config
   const TpchConfig& tpchConfig;

   /// The processing thread
   std::thread thread;
   /// Whether we are done
   std::atomic<bool> done;

   /// Perform work
   void performWork();

   public:
   /// Constructor
   explicit TpchRefreshClient(const ClientConfig& clientConfig, const TpchConfig& tpchConfig);

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