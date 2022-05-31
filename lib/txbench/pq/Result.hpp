#ifndef H_lib_txbench_pq_Result
#define H_lib_txbench_pq_Result
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
struct pg_result;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
/// RAII wrapper for pg_result objects
class Result {
   protected:
   /// The result object
   pg_result* res;
   /// The status code indicating success
   int successCode;

   public:
   /// Constructor
   Result(pg_result* res, int successCode);
   /// Destructor
   ~Result();

   /// Get the status
   Status getStatus() const;

   /// Get the raw result object
   pg_result* get() { return res; }
   /// Get the raw result object
   const pg_result* get() const { return res; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif