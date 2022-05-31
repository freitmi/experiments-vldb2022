#include "client/ClientConfig.hpp"
#include "txbench/io/JsonParser.hpp"
#include "txbench/pq/ConnectionString.hpp"
#include <sstream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
Status ClientConfig::read(const string& filename)
// Read a configuration file
{
   JsonParser parser;
   if (auto res = parser.parse(filename); !res)
      return res;

   parser.map("host", host);
   parser.map("port", port);
   parser.map("username", username);
   parser.map("password", password);
   parser.map("schema_scripts", schemaScripts);
   parser.map("load_scripts", loadScripts);
   parser.map("pipeline_depth", pipelineDepth);

   auto validate = [&]() {
      if (parser.hasError())
         return false;

      return true;
   };

   return validate() ? Status::Success : Status::Error("unable to parse client config");
}
//---------------------------------------------------------------------------
pq::ConnectionString ClientConfig::getConnectionString() const
// Get the connection string
{
   ostringstream result;
   if (!host.empty())
      result << "host=" << host << " ";
   if (!port.empty())
      result << "port=" << port << " ";
   if (!username.empty())
      result << "user=" << username << " ";
   if (!password.empty())
      result << "password=" << password << " ";

   return pq::ConnectionString{result.str()};
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------