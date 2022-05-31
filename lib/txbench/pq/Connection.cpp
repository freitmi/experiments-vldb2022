#include "txbench/pq/Connection.hpp"
#include "txbench/Status.hpp"
#include "txbench/pq/ConnectionString.hpp"
#include "txbench/pq/ResultSet.hpp"
#include <cassert>
#include <thread>
#include <libpq-fe.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
Connection::Connection()
   : con(nullptr)
// Constructor
{
}
//---------------------------------------------------------------------------
Connection::~Connection()
// Destructor
{
   close();
}
//---------------------------------------------------------------------------
Status Connection::connect(const ConnectionString& connectionString, unsigned timeout)
// Attempt to connect
{
   close();

   Status result = Status::Success;
   auto endTime = chrono::steady_clock::now() + chrono::seconds(timeout);
   while (chrono::steady_clock::now() < endTime) {
      con = PQconnectdb(connectionString.get().c_str());
      if (auto res = PQstatus(con); res == CONNECTION_OK) {
         result = Status::Success;
         break;
      }

      // Remember the last error
      result = Status::Error(PQerrorMessage(con));

      // And retry
      close();
      this_thread::sleep_for(100ms);
   }

   return result;
}
//---------------------------------------------------------------------------
void Connection::close()
// Close the connection
{
   if (con) {
      PQfinish(con);
      con = nullptr;
   }
}
//---------------------------------------------------------------------------
ResultSet Connection::executeQuery(const string& query)
// Execute an ad-hoc query
{
   return ResultSet{PQexec(con, query.c_str()), PGRES_TUPLES_OK};
}
//---------------------------------------------------------------------------
Result Connection::executeCommand(const string& command)
// Execute an ad-hoc query
{
   return Result{PQexec(con, command.c_str()), PGRES_COMMAND_OK};
}
//---------------------------------------------------------------------------
Status Connection::enterPipelineMode()
// Enter pipeline mode
{
   assert(!pipelineMode);

   if (!PQenterPipelineMode(con))
      return Status::Error(PQerrorMessage(con));

   pipelineMode = true;
   return Status::Success;
}
//---------------------------------------------------------------------------
Status Connection::sync()
// Synchronize the pipeline
{
   assert(pipelineMode);

   if (!PQpipelineSync(con))
      return Status::Error(PQerrorMessage(con));

   while (PQgetResult(con))
      ;

   return Status::Success;
}
//---------------------------------------------------------------------------
Status Connection::exitPipelineMode()
// Exit pipeline mode
{
   assert(pipelineMode);

   if (!PQexitPipelineMode(con))
      return Status::Error(PQerrorMessage(con));

   pipelineMode = false;
   return Status::Success;
}
//---------------------------------------------------------------------------
Status Connection::flush()
// Flush the pipeline
{
   assert(pipelineMode);

   if (PQflush(con) != 0)
      return Status::Error(PQerrorMessage(con));
   return Status::Success;
}
//---------------------------------------------------------------------------
Status Connection::enqueueFlushRequest()
// Enqueue a flush request
{
   assert(pipelineMode);

   if (!PQsendFlushRequest(con))
      return Status::Error(PQerrorMessage(con));
   return Status::Success;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------