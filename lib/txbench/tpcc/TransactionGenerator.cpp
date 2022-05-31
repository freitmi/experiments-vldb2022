#include "txbench/tpcc/TransactionGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tpcc {
//---------------------------------------------------------------------------
TransactionGenerator::TransactionGenerator(uint64_t seed)
   : generator(seed)
// Constructor
{
}
//---------------------------------------------------------------------------
TransactionGenerator::TransactionType TransactionGenerator::pickTransactionType()
// Pick a transaction type
{
   auto dice = generator.generateRand(0, 100);
   if (dice < 43)
      return TransactionType::Payment;

   dice -= 43;
   if (dice < 4)
      return TransactionType::OrderStatus;

   dice -= 4;
   if (dice < 4)
      return TransactionType::Delivery;

   dice -= 4;
   if (dice < 4)
      return TransactionType::StockLevel;

   dice -= 4;
   return TransactionType::NewOrder;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------