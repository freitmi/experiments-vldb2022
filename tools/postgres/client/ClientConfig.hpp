#ifndef H_tools_postgres_client_ClientConfig
#define H_tools_postgres_client_ClientConfig
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
#include <string>
#include <vector>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
namespace pq {
//---------------------------------------------------------------------------
class ConnectionString;
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
namespace postgres {
//---------------------------------------------------------------------------
/// Client configuration
class ClientConfig {
   private:
   /// The database host
   std::string host;
   /// The database port
   std::string port;

   /// The username
   std::string username;
   /// The password
   std::string password;

   /// The schema scripts
   std::vector<std::string> schemaScripts;
   /// The load scripts
   std::vector<std::string> loadScripts;

   /// The pipeline depth
   unsigned pipelineDepth;

   public:
   /// Read a configuration file
   Status read(const std::string& filename);

   /// Get the connection string
   pq::ConnectionString getConnectionString() const;

   /// Get the schema scripts
   const std::vector<std::string>& getSchemaScripts() const { return schemaScripts; }
   /// Get the load scripts
   const std::vector<std::string>& getLoadScripts() const { return loadScripts; }

   /// Get the pipeline depth
   unsigned getPipelineDepth() const { return pipelineDepth; }
};
//---------------------------------------------------------------------------
}
}
//---------------------------------------------------------------------------
#endif