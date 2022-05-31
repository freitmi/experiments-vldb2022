#ifndef H_tools_umbra_client_load_LoadConfig
#define H_tools_umbra_client_load_LoadConfig
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// Configuration for load tests
class LoadConfig {
   private:
   /// Whether to use bulk operations
   bool bulkOperations;

   public:
   /// Read a configuration file
   Status read(const std::string& filename);

   /// Check whether we should use bulk operations
   bool useBulkOperations() const { return bulkOperations; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif