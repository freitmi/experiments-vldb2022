#include "client/Client.hpp"
#include "txbench/pq/ConnectionString.hpp"
#include "txbench/pq/Result.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
Client::Client(const ClientConfig& clientConfig)
   : clientConfig(clientConfig)
// Constructor
{
}
//---------------------------------------------------------------------------
Client::~Client()
// Destructor
{
}
//---------------------------------------------------------------------------
uint64_t Client::getSubmitted() const
// Get the number of submitted transactions
{
   return submitted.load(memory_order_acquire);
}
//---------------------------------------------------------------------------
uint64_t Client::getProcessed() const
// Get the number of processed transactions
{
   return processed.load(memory_order_acquire);
}
//---------------------------------------------------------------------------
Status Client::initialize()
// Initialize the client
{
   if (auto res = connection.connect(clientConfig.getConnectionString()); !res)
      return res;

   {
      // Set the commit behavior
      auto result = connection.executeCommand("SET async_commit = " + clientConfig.getAsyncCommit());
      if (auto res = result.getStatus(); !res)
         return res;
   }

   {
      // Set the call statement behavior
      auto result = connection.executeCommand("SET call_error_handling = " + clientConfig.getCallErrorHandling());
      if (auto res = result.getStatus(); !res)
         return res;
   }

   return Status::Success;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------