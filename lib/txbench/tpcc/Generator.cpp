#include "txbench/tpcc/Generator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::tpcc {
//---------------------------------------------------------------------------
Generator::Generator(uint64_t seed)
   : rng(seed)
// Constructor
{
}
//---------------------------------------------------------------------------
mt19937& Generator::getEngine()
// Get the underlying engine
{
   return rng;
}
//---------------------------------------------------------------------------
string Generator::generateNumString(unsigned length)
// Generate a random n-string
{
   return generateNumString(length, length);
}
//---------------------------------------------------------------------------
string Generator::generateNumString(unsigned minLength, unsigned maxLength)
// Generate a random n-string
{
   uniform_int_distribution lengthDist(minLength, maxLength);
   uniform_int_distribution charDist(0, 9);

   string result(lengthDist(rng), ' ');
   for (auto& c : result)
      c = numeric[charDist(rng)];
   return result;
}
//---------------------------------------------------------------------------
string Generator::generateAlphanumString(unsigned length)
// Generate a random a-string
{
   return generateAlphanumString(length, length);
}
//---------------------------------------------------------------------------
string Generator::generateAlphanumString(unsigned minLength, unsigned maxLength)
// Generate a random a-string
{
   uniform_int_distribution lengthDist(minLength, maxLength);
   uniform_int_distribution charDist(0, 61);

   string result(lengthDist(rng), ' ');
   for (auto& c : result)
      c = alphanumeric[charDist(rng)];
   return result;
}
//---------------------------------------------------------------------------
string Generator::generateZipCode()
// Generate a random zip code
{
   uniform_int_distribution charDist(0, 9);

   string result(9, '1');
   for (unsigned i = 0; i < 4; ++i)
      result[i] = numeric[charDist(rng)];
   return result;
}
//---------------------------------------------------------------------------
string Generator::generateData()
// Generate a random data string
{
   uniform_real_distribution originalDist;

   string data = generateAlphanumString(26, 50);
   if (originalDist(rng) < 0.1) {
      uniform_int_distribution offsetDist(0ull, data.size() - 8ull);
      data.replace(offsetDist(rng), 8, "ORIGINAL");
   }

   return data;
}
//---------------------------------------------------------------------------
string Generator::generateCustomerLastname(unsigned index)
// Make a customer lastname
{
   unsigned index1 = index % 10;
   unsigned index2 = (index / 10) % 10;
   unsigned index3 = (index / 100) % 10;

   string result(lastnameSyllables[index1].size() + lastnameSyllables[index2].size() + lastnameSyllables[index3].size(), ' ');
   result.replace(0, lastnameSyllables[index1].size(), lastnameSyllables[index1]);
   result.replace(lastnameSyllables[index1].size(), lastnameSyllables[index2].size(), lastnameSyllables[index2]);
   result.replace(lastnameSyllables[index1].size() + lastnameSyllables[index2].size(), lastnameSyllables[index3].size(), lastnameSyllables[index3]);

   return result;
}
//---------------------------------------------------------------------------
unsigned int Generator::generateRand(unsigned min, unsigned max)
// Generate a uniform random number
{
   uniform_int_distribution dist(min, max);
   return dist(rng);
}
//---------------------------------------------------------------------------
unsigned Generator::generateNURand(unsigned A, unsigned min, unsigned max)
// Generate a non-uniform random number
{
   uniform_int_distribution dist1(0u, A);
   uniform_int_distribution dist2(min, max);

   return (((dist1(rng) | dist2(rng)) + 42) % (max - min + 1)) + min;
}
//---------------------------------------------------------------------------
string Generator::generateDatetime()
// Generate the current date and time
{
   time_t now = {};
   time(&now);
   string result(19, ' ');
   strftime(&result[0], 20, "%Y-%m-%d %H:%M:%S", gmtime(&now));
   return result;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------