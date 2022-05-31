#ifndef H_lib_txbench_tatp_Generator
#define H_lib_txbench_tatp_Generator
//---------------------------------------------------------------------------
#include <array>
#include <cstdint>
#include <random>
#include <span>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tatp {
//---------------------------------------------------------------------------
/// Bundles generic generation functionality for TATP, used both by the data generator and the driver programs
class Generator {
   public:
   /// Sample generator
   class SampleGenerator {
      private:
      /// The generator
      Generator* generator;
      /// The possible values
      std::vector<uint64_t> values;

      /// Constructor
      SampleGenerator(Generator& generator, std::vector<uint64_t> values);

      friend class Generator;

      public:
      /// Sample values
      std::span<uint64_t> sample(uint64_t count);
   };

   private:
   /// The random number generator
   std::mt19937 rng;

   /// Alphabetic
   static constexpr std::string_view alphabetic = std::string_view("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);

   public:
   /// Constructor
   explicit Generator(uint64_t seed);

   /// Get a sample generator
   SampleGenerator getSampleGenerator(std::vector<uint64_t> values);

   /// Generate a uniform random number
   uint64_t generateRand(uint64_t min, uint64_t max);
   /// Generate a digit string from a number
   std::string generateDigitString(uint64_t number, uint64_t digits);
   /// Generate a random alphabetical string
   std::string generateAlphaString(uint64_t length);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif