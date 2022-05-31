#ifndef H_lib_txbench_pq_ResultSet
#define H_lib_txbench_pq_ResultSet
//---------------------------------------------------------------------------
#include "txbench/pq/Result.hpp"
#include <string_view>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
/// A result set
class ResultSet : public Result {
   public:
   /// Constructor
   explicit ResultSet(pg_result* res, int successCode);

   /// Get the number of columns
   unsigned getColumns() const;
   /// Get the number of rows
   unsigned getRows() const;

   /// Get a non-null field
   std::string_view get(unsigned row, unsigned column);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif