#include "txbench/tatp/Generator.hpp"
#include <iomanip>
#include <sstream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::tatp {
//---------------------------------------------------------------------------
Generator::SampleGenerator::SampleGenerator(Generator& generator, vector<uint64_t> values)
   : generator(&generator), values(move(values))
// Constructor
{
}
//---------------------------------------------------------------------------
span<uint64_t> Generator::SampleGenerator::sample(uint64_t count)
// Generate
{
   shuffle(values.begin(), values.end(), generator->rng);
   return {values.data(), count};
}
//---------------------------------------------------------------------------
Generator::Generator(uint64_t seed)
   : rng(seed)
// Constructor
{
}
//---------------------------------------------------------------------------
Generator::SampleGenerator Generator::getSampleGenerator(vector<uint64_t> values)
// Get a sample generator
{
   return SampleGenerator{*this, move(values)};
}
//---------------------------------------------------------------------------
uint64_t Generator::generateRand(uint64_t min, uint64_t max)
// Generate a uniform random number
{
   uniform_int_distribution dist(min, max);
   return dist(rng);
}
//---------------------------------------------------------------------------
string Generator::generateDigitString(uint64_t number, uint64_t digits)
// Generate a digit string
{
   ostringstream out;
   out << setw(digits) << setfill('0') << number;
   return out.str();
}
//---------------------------------------------------------------------------
string Generator::generateAlphaString(uint64_t length)
// Generate a random alphabetic string
{
   uniform_int_distribution charDist(0, static_cast<int>(alphabetic.size() - 1));

   string result(length, ' ');
   for (auto& c : result)
      c = alphabetic[charDist(rng)];
   return result;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
