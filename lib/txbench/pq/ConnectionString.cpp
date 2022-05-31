#include "txbench/pq/ConnectionString.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
ConnectionString::ConnectionString(string connectionString)
   : connectionString(move(connectionString))
// Constructor
{
}
//---------------------------------------------------------------------------
const string& ConnectionString::get() const
// Get the connection string
{
   return connectionString;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------