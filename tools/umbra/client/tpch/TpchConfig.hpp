#ifndef H_tools_umbra_client_tpch_TpchConfig
#define H_tools_umbra_client_tpch_TpchConfig
//---------------------------------------------------------------------------
#include "client/DriverConfig.hpp"
#include "txbench/Status.hpp"
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// TPCH configuration
class TpchConfig : public DriverConfig {
   private:
   /// Whether we should refresh data
   bool refresh;
   /// Whether we should use bulk operations
   bool bulkOperations;

   public:
   /// Read a configuration file
   Status read(const std::string& filename);

   /// Check whether we should refresh data
   bool refreshData() const { return refresh; }
   /// Check whether we should use bulk operations
   bool useBulkOperations() const { return bulkOperations; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif