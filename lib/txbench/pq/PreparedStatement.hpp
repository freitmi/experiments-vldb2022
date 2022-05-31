#ifndef H_lib_txbench_pq_PreparedStatement
#define H_lib_txbench_pq_PreparedStatement
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
#include "txbench/pq/AsyncCall.hpp"
#include "txbench/pq/Network.hpp"
#include <cstring>
#include <string>
#include <vector>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
struct pg_conn;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
class Connection;
class Result;
class ResultSet;
//---------------------------------------------------------------------------
/// A prepared statement
class PreparedStatement {
   private:
   /// The database connection object
   Connection* con = nullptr;
   /// The name of the prepared statment
   std::string name;
   /// The number of parameters of the prepared statement
   int numParams = 0;

   /// The parameter value pointers
   std::vector<const char*> paramValues;
   /// The parameter value buffer
   std::vector<char> paramValuesBuffer;
   /// The parameter lengths
   std::vector<int> paramLengths;
   /// The parameter formats
   std::vector<int> paramFormats;

   /// Serialize parameters
   template <typename... Tail>
   void serialize(unsigned index, int head, Tail&&... tail);
   /// Send the query with the current parameter values
   Status send();
   /// Receive a command result
   Result receiveCommand();
   /// Receive a query result
   ResultSet receiveQuery();

   friend class AsyncCall;

   public:
   /// Constructor
   explicit PreparedStatement();
   /// Destructor
   ~PreparedStatement();

   /// Prepare a statement
   Status prepare(Connection& connection, const std::string& query);

   /// Bind parameter values and prepare an asynchronous call
   template <typename... Args>
   AsyncCall bind(Args&&... args);
};
//---------------------------------------------------------------------------
template <typename... Tail>
void PreparedStatement::serialize(unsigned index, int head, Tail&&... tail)
// Serialize parameters
{
   auto arg = Network::host2be(head);
   memcpy(&paramValuesBuffer[index * sizeof(int)], &arg, sizeof(int));

   if constexpr (sizeof...(tail) > 0)
      serialize(index + 1, tail...);
}
//---------------------------------------------------------------------------
template <typename... Args>
AsyncCall PreparedStatement::bind(Args&&... args)
// Call the procedure
{
   if constexpr (sizeof...(args) > 0)
      serialize(0, args...);
   return AsyncCall(this);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif