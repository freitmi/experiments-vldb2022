#ifndef H_lib_txbench_tatp_TransactionGenerator
#define H_lib_txbench_tatp_TransactionGenerator
//---------------------------------------------------------------------------
#include "txbench/tatp/Generator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tatp {
//---------------------------------------------------------------------------
/// Transaction type generator
class TransactionGenerator {
   public:
   /// The transaction types
   enum class TransactionType {
      GetSubscriberData = 0,
      GetNewDestination = 1,
      GetAccessData = 2,
      UpdateSubscriberData = 3,
      UpdateLocation = 4,
      InsertCallForwarding = 5,
      DeleteCallForwarding = 6
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