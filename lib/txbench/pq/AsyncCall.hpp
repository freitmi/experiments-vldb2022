#ifndef H_lib_txbench_pq_AsyncCall
#define H_lib_txbench_pq_AsyncCall
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
class PreparedStatement;
class Result;
class ResultSet;
//---------------------------------------------------------------------------
/// Wrapper for an asynchronous procedure call
class AsyncCall {
   private:
   /// The prepared statement
   PreparedStatement* statement;

   /// Constructor
   explicit AsyncCall(PreparedStatement* statement);

   friend class PreparedStatement;

   public:
   /// Constructor
   AsyncCall();

   /// Send the query to the server
   Status send();
   /// Receive the command from the server
   Result receiveCommand();
   /// Receive the query result from the server
   ResultSet receiveQuery();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif