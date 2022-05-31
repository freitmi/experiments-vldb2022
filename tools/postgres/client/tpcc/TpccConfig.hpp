#ifndef H_tools_postgres_client_tpcc_TpccConfig
#define H_tools_postgres_client_tpcc_TpccConfig
//---------------------------------------------------------------------------
#include "client/DriverConfig.hpp"
#include "txbench/Status.hpp"
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// TPCC configuration
class TpccConfig : public DriverConfig {
   private:
   /// Whether to generate remote accesses
   bool generateRemoteAccesses;
   /// Whether to generate user rollbacks
   bool generateUserRollbacks;

   public:
   /// Read a configuration file
   Status read(const std::string& filename);

   /// Whether to generate remote accesses
   bool shouldGenerateRemoteAccesses() const { return generateRemoteAccesses; }
   /// Whether to generate user rollbacks
   bool shouldGenerateUserRollbacks() const { return generateUserRollbacks; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif