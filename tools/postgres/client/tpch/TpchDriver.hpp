#ifndef H_tools_postgres_client_tpch_TpchDriver
#define H_tools_postgres_client_tpch_TpchDriver
//---------------------------------------------------------------------------
#include "client/Driver.hpp"
#include "client/tpch/TpchConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// TPCH driver
class TpchDriver : public Driver {
   private:
   /// The TPCH config
   const TpchConfig& tpchConfig;

   protected:
   /// Create a client
   std::unique_ptr<Client> createClient(unsigned int clientIndex) const override;

   public:
   /// Constructor
   TpchDriver(const ClientConfig& clientConfig, const TpchConfig& tpchConfig, unsigned clientCount, std::string logfile);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif