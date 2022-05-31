#ifndef H_tools_postgres_client_tatp_TatpDriver
#define H_tools_postgres_client_tatp_TatpDriver
//---------------------------------------------------------------------------
#include "client/Driver.hpp"
#include "client/tatp/TatpConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// TATP driver
class TatpDriver : public Driver {
   private:
   /// The TATP config
   const TatpConfig& tatpConfig;

   protected:
   /// Create a client
   std::unique_ptr<Client> createClient(unsigned int clientIndex) const override;

   public:
   /// Constructor
   TatpDriver(const ClientConfig& clientConfig, const TatpConfig& tatpConfig, unsigned clientCount, std::string logfile);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif