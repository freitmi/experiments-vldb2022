#ifndef H_tools_postgres_client_tpch_TpchConfig
#define H_tools_postgres_client_tpch_TpchConfig
//---------------------------------------------------------------------------
#include "client/DriverConfig.hpp"
#include "txbench/Status.hpp"
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// TPCH configuration
class TpchConfig : public DriverConfig {
   private:
   /// Whether we should refresh data
   bool refresh;

   public:
   /// Read a configuration file
   Status read(const std::string& filename);

   /// Check whether we should refresh data
   bool refreshData() const { return refresh; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif