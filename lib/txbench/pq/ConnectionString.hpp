#ifndef H_lib_txbench_pq_ConnectionString
#define H_lib_txbench_pq_ConnectionString
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
/// A connection string
class ConnectionString {
   private:
   /// The connection string
   std::string connectionString;

   public:
   /// Constructor
   explicit ConnectionString(std::string connectionString);

   /// Get the connection string
   const std::string& get() const;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif