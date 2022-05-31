#include "txbench/tatp/TransactionGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::tatp {
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
   if (dice <= 35)
      return TransactionType::GetSubscriberData;

   dice -= 35;
   if (dice <= 35)
      return TransactionType::GetAccessData;

   dice -= 35;
   if (dice <= 14)
      return TransactionType::UpdateLocation;

   dice -= 14;
   if (dice <= 10)
      return TransactionType::GetNewDestination;

   dice -= 10;
   if (dice <= 2)
      return TransactionType::UpdateSubscriberData;

   dice -= 2;
   if (dice <= 2)
      return TransactionType::InsertCallForwarding;

   dice -= 2;
   return TransactionType::DeleteCallForwarding;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
