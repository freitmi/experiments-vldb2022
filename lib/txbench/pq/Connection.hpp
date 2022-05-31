#ifndef H_lib_txbench_pq_Connection
#define H_lib_txbench_pq_Connection
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
struct pg_conn;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
class ConnectionString;
class Result;
class ResultSet;
//---------------------------------------------------------------------------
/// A database connection
class Connection {
   private:
   /// The database connection object
   pg_conn* con;
   /// Whether we are in pipeline mode
   bool pipelineMode = false;

   friend class PreparedStatement;

   public:
   /// Constructor
   Connection();
   /// Destructor
   ~Connection();

   /// Attempt to connect
   Status connect(const ConnectionString& connectionString, unsigned timeout = 5);
   /// Close the connection
   void close();

   /// Execute an ad-hoc query
   ResultSet executeQuery(const std::string& query);
   /// Execute an ad-hoc command
   Result executeCommand(const std::string& command);

   /// Check whether we are in pipeline mode
   bool isInPipelineMode() const { return pipelineMode; }
   /// Enter pipeline mode
   Status enterPipelineMode();
   /// Synchronize the pipeline
   Status sync();
   /// Exit pipeline mode
   Status exitPipelineMode();

   /// Flush the pipeline
   Status flush();
   /// Enqueue a request that the server flushes its pipeline
   Status enqueueFlushRequest();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif