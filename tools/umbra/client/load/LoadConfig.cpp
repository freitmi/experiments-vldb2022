#include "client/load/LoadConfig.hpp"
#include "txbench/io/JsonParser.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
Status LoadConfig::read(const string& filename)
// Read a configuration file
{
   JsonParser parser;
   if (auto res = parser.parse(filename); !res)
      return res;

   parser.map("bulk_operations", bulkOperations);

   auto validate = [&]() {
      if (parser.hasError())
         return false;

      return true;
   };

   return validate() ? Status::Success : Status::Error("unable to parse loadtest config");
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------