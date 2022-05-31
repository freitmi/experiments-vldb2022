#include "client/tpcc/TpccDriver.hpp"
#include "client/tpcc/TpccClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
TpccDriver::TpccDriver(const ClientConfig& clientConfig, const TpccConfig& tpccConfig, unsigned clientCount, string logfile)
   : Driver(clientConfig, tpccConfig, clientCount, move(logfile)), tpccConfig(tpccConfig)
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<Client> TpccDriver::createClient(unsigned clientIndex) const
// Create a client
{
   return make_unique<TpccClient>(clientConfig, tpccConfig, clientIndex + 1);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
