#ifndef H_tools_postgres_client_tpcc_TpccDriver
#define H_tools_postgres_client_tpcc_TpccDriver
//---------------------------------------------------------------------------
#include "client/Driver.hpp"
#include "client/tpcc/TpccConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// TPCC driver
class TpccDriver : public Driver {
   private:
   /// The TPCC config
   const TpccConfig& tpccConfig;

   protected:
   /// Create a client
   std::unique_ptr<Client> createClient(unsigned int clientIndex) const override;

   public:
   /// Constructor
   TpccDriver(const ClientConfig& clientConfig, const TpccConfig& tpccConfig, unsigned clientCount, std::string logfile);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif