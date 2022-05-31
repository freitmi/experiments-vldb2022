#include "client/Client.hpp"
#include "txbench/pq/ConnectionString.hpp"
#include "txbench/pq/Result.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
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
   return connection.connect(clientConfig.getConnectionString());
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------