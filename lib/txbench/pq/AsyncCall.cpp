#include "txbench/pq/AsyncCall.hpp"
#include "txbench/pq/PreparedStatement.hpp"
#include "txbench/pq/ResultSet.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
AsyncCall::AsyncCall()
   : statement(nullptr)
// Constructor
{
}
//---------------------------------------------------------------------------
AsyncCall::AsyncCall(PreparedStatement* statement)
   : statement(statement)
// Constructor
{
}
//---------------------------------------------------------------------------
Status AsyncCall::send()
// Send the statement
{
   return statement->send();
}
//---------------------------------------------------------------------------
Result AsyncCall::receiveCommand()
// Receive the command result
{
   return statement->receiveCommand();
}
//---------------------------------------------------------------------------
ResultSet AsyncCall::receiveQuery()
// Receive the query result
{
   return statement->receiveQuery();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------