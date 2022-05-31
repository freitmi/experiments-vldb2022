#include "client/tpcc/TpccConfig.hpp"
#include "txbench/io/JsonParser.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
Status TpccConfig::read(const string& filename)
// Read a configuration file
{
   JsonParser parser;
   if (auto res = parser.parse(filename); !res)
      return res;

   parser.map("warmup_time", warmupTime);
   parser.map("measure_time", measureTime);
   parser.map("generate_remote_accesses", generateRemoteAccesses);
   parser.map("generate_user_rollbacks", generateUserRollbacks);

   auto validate = [&]() {
      if (parser.hasError())
         return false;

      return true;
   };

   return validate() ? Status::Success : Status::Error("unable to parse driver config");
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------