#ifndef H_tools_postgres_client_tatp_TatpConfig
#define H_tools_postgres_client_tatp_TatpConfig
//---------------------------------------------------------------------------
#include "client/DriverConfig.hpp"
#include "txbench/Status.hpp"
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// TATP configuration
class TatpConfig : public DriverConfig {
   public:
   /// Read a configuration file
   Status read(const std::string& filename);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif