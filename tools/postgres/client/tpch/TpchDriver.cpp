#include "client/tpch/TpchDriver.hpp"
#include "client/tpch/TpchQueryClient.hpp"
#include "client/tpch/TpchRefreshClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
TpchDriver::TpchDriver(const ClientConfig& clientConfig, const TpchConfig& tpchConfig, unsigned clientCount, string logfile)
   : Driver(clientConfig, tpchConfig, clientCount, move(logfile)), tpchConfig(tpchConfig)
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<Client> TpchDriver::createClient(unsigned clientIndex) const
// Create a client
{
   if (tpchConfig.refreshData()) {
      if (clientIndex == 0)
         return make_unique<TpchRefreshClient>(clientConfig, tpchConfig);
      if (clientIndex == 1)
         return make_unique<TpchQueryClient>(clientConfig, tpchConfig);
   } else if (clientIndex == 0) {
      return make_unique<TpchQueryClient>(clientConfig, tpchConfig);
   }

   return nullptr;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------