#include "txbench/pq/PreparedStatement.hpp"
#include "txbench/Status.hpp"
#include "txbench/pq/Connection.hpp"
#include "txbench/pq/ResultSet.hpp"
#include <atomic>
#include <cassert>
#include <libpq-fe.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
static atomic<unsigned> id = 0;
//---------------------------------------------------------------------------
PreparedStatement::PreparedStatement()
// Constructor
{
}
//---------------------------------------------------------------------------
PreparedStatement::~PreparedStatement()
// Destructor
{
}
//---------------------------------------------------------------------------
Status PreparedStatement::prepare(Connection& connection, const std::string& query)
// Prepare the statement
{
   assert(!con);

   con = &connection;
   name = "prepared_" + to_string(id.fetch_add(1));

   {
      Result result(PQprepare(con->con, name.c_str(), query.c_str(), 0, nullptr), PGRES_COMMAND_OK);
      if (auto res = result.getStatus(); !res)
         return res;
   }

   {
      Result result(PQdescribePrepared(con->con, name.c_str()), PGRES_COMMAND_OK);
      if (auto res = result.getStatus(); !res)
         return res;

      numParams = PQnparams(result.get());
   }

   paramValues.resize(numParams);
   paramValuesBuffer.resize(sizeof(int) * numParams);
   paramLengths.resize(numParams);
   paramFormats.resize(numParams);

   for (unsigned i = 0; i < numParams; ++i) {
      paramValues[i] = &paramValuesBuffer[sizeof(int) * i];
      paramLengths[i] = sizeof(int);
      paramFormats[i] = 1;
   }

   return Status::Success;
}
//---------------------------------------------------------------------------
Status PreparedStatement::send()
// Call the procedure
{
   if (!PQsendQueryPrepared(con->con, name.c_str(), numParams, paramValues.data(), paramLengths.data(), paramFormats.data(), 0))
      return Status::Error(PQerrorMessage(con->con));

   return Status::Success;
}
//---------------------------------------------------------------------------
Result PreparedStatement::receiveCommand()
// Receive a command result
{
   // Get the actual result
   auto first = Result{PQgetResult(con->con), PGRES_COMMAND_OK};

   // PQgetResult has to be called until it returns nullptr.
   while (PQgetResult(con->con))
      ;

   return first;
}
//---------------------------------------------------------------------------
ResultSet PreparedStatement::receiveQuery()
// Receive a query result
{
   // Get the actual result
   auto first = ResultSet{PQgetResult(con->con), PGRES_TUPLES_OK};

   // PQgetResult has to be called until it returns nullptr.
   while (PQgetResult(con->con))
      ;

   return first;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------