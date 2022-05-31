#ifndef H_tools_datagen_generator_TpccGenerator
#define H_tools_datagen_generator_TpccGenerator
//---------------------------------------------------------------------------
#include "txbench/tpcc/Generator.hpp"
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
class TpccGenerator {
   private:
   /// The scale factor
   unsigned scaleFactor;
   /// The output directory
   const std::string& basedir;

   /// The generator
   tpcc::Generator generator;

   /// Generate the item table
   void generateItem();
   /// Generate the warehouse table
   void generateWarehouse();
   /// Generate the stock table
   void generateStock();
   /// Generate the district table
   void generateDistrict();
   /// Generate the customer table
   void generateCustomer();
   /// Generate the history table
   void generateHistory();
   /// Generate the order tables
   void generateOrders();

   public:
   /// Constructor
   TpccGenerator(unsigned scaleFactor, const std::string& basedir);

   /// Generate data
   void generate();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif