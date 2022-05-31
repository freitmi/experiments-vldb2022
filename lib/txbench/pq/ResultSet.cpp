#include "txbench/pq/ResultSet.hpp"
#include <cassert>
#include <libpq-fe.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
ResultSet::ResultSet(pg_result* res, int successCode)
   : Result(res, successCode)
// Constructor
{
}
//---------------------------------------------------------------------------
unsigned ResultSet::getColumns() const
// Get the number of columns
{
   return PQnfields(res);
}
//---------------------------------------------------------------------------
unsigned ResultSet::getRows() const
// Get the number of rows
{
   return PQntuples(res);
}
//---------------------------------------------------------------------------
string_view ResultSet::get(unsigned row, unsigned column)
// Get a field
{
   assert(row < getRows());
   assert(column < getColumns());

   return string_view(PQgetvalue(res, row, column), PQgetlength(res, row, column));
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------