#ifndef H_lib_txbench_tpcc_TransactionGenerator
#define H_lib_txbench_tpcc_TransactionGenerator
//---------------------------------------------------------------------------
#include "txbench/tpcc/Generator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tpcc {
//---------------------------------------------------------------------------
/// Transaction type generator
class TransactionGenerator {
   public:
   /// The transaction types
   enum class TransactionType {
      Delivery = 0,
      NewOrder = 1,
      OrderStatus = 2,
      Payment = 3,
      StockLevel = 4
   };

   private:
   /// The underlying data generator
   Generator generator;

   public:
   /// Constructor
   explicit TransactionGenerator(uint64_t seed);

   /// Pick a transaction type
   TransactionType pickTransactionType();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif