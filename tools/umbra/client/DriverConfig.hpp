#ifndef H_tools_umbra_client_DriverConfig
#define H_tools_umbra_client_DriverConfig
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// Driver configuration
class DriverConfig {
   protected:
   /// The warmup time
   unsigned warmupTime;
   /// The measurement time
   unsigned measureTime;

   public:
   /// Get the warmup time
   unsigned getWarmupTime() const { return warmupTime; }
   /// Get the measurement time
   unsigned getMeasureTime() const { return measureTime; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif