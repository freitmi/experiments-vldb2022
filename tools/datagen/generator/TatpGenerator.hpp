#ifndef H_tools_datagen_generator_TatpGenerator
#define H_tools_datagen_generator_TatpGenerator
//---------------------------------------------------------------------------
#include "txbench/tatp/Generator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
/// TATP data generator
class TatpGenerator {
   private:
   /// The scale factor
   unsigned scaleFactor;
   /// The output directory
   const std::string& basedir;

   /// The generator
   tatp::Generator generator;

   /// Generate subscriber data
   void generateSubscriber();
   /// Generate access info data
   void generateAccessInfo();
   /// Generate special facility and call forwarding data
   void generateSpecialFacility();

   public:
   /// Constructor
   TatpGenerator(unsigned scaleFactor, const std::string& basedir);

   /// Generate data
   void generate();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif