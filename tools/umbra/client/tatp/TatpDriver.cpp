#include "client/tatp/TatpDriver.hpp"
#include "client/tatp/TatpClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
TatpDriver::TatpDriver(const ClientConfig& clientConfig, const TatpConfig& tatpConfig, unsigned clientCount, string logfile)
   : Driver(clientConfig, tatpConfig, clientCount, move(logfile)), tatpConfig(tatpConfig)
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<Client> TatpDriver::createClient(unsigned clientIndex) const
// Create a client
{
   return make_unique<TatpClient>(clientConfig, tatpConfig, clientIndex + 1);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
