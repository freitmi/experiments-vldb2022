#include "txbench/pq/Result.hpp"
#include <cassert>
#include <libpq-fe.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
Result::Result(pg_result* res, int successCode)
   : res(res), successCode(successCode)
// Constructor
{
}
//---------------------------------------------------------------------------
Result::~Result()
// Destructor
{
   if (res)
      PQclear(res);
}
//---------------------------------------------------------------------------
Status Result::getStatus() const
// Get the result status
{
   assert(res);

   if (PQresultStatus(res) != successCode)
      return Status::Error(PQresultErrorMessage(res));

   return Status::Success;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------