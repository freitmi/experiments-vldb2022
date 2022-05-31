#include "client/Driver.hpp"
#include "client/Client.hpp"
#include "txbench/io/CsvWriter.hpp"
#include <iomanip>
#include <iostream>
#include <thread>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
namespace {
//---------------------------------------------------------------------------
string formatTime(unsigned seconds)
// Print a time
{
   stringstream out;
   out << setfill('0') << setw(2) << (seconds / 60);
   out << ":";
   out << setfill('0') << setw(2) << (seconds % 60);
   return out.str();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
Driver::Driver(const ClientConfig& clientConfig, const DriverConfig& driverConfig, unsigned clientCount, string logfile)
   : clientConfig(clientConfig), driverConfig(driverConfig), clientCount(clientCount), logfile(move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
Driver::~Driver()
// Destructor
{
}
//---------------------------------------------------------------------------
uint64_t Driver::countSubmitted()
// Count the number of submitted transactions
{
   uint64_t result = 0;
   for (const auto& client : clients)
      result += client->getSubmitted();
   return result;
}
//---------------------------------------------------------------------------
uint64_t Driver::countProcessed()
// Count the number of processed transactions
{
   uint64_t result = 0;
   for (const auto& client : clients)
      result += client->getProcessed();
   return result;
}
//---------------------------------------------------------------------------
bool Driver::run()
// Run the driver
{
   {
      auto beginTime = chrono::steady_clock::now();

      // Initialize clients
      clients.reserve(clientCount);
      for (unsigned i = 0; i < clientCount; ++i) {
         auto& client = clients.emplace_back(createClient(i));
         if (auto res = client->initialize(); !res) {
            cerr << "unable to initialize client: " << res.getMessage() << endl;
            return false;
         }
      }

      auto endTime = chrono::steady_clock::now();

      cerr << "Initialized " << clientCount << " " << ((clientCount > 1) ? "clients" : "client") << " in " << (1e-9 * (endTime - beginTime).count()) << " s" << endl;
      cerr << endl;
   }

   // Start processing
   for (auto& client : clients)
      client->start();

   auto warmupTime = driverConfig.getWarmupTime();
   auto measureTime = driverConfig.getMeasureTime();

   thread monitorThread([&]() {
      CsvWriter logWriter(logfile);

      if (warmupTime) {
         auto beginTime = chrono::high_resolution_clock::now();
         auto processed = countProcessed();
         for (unsigned i = 1; i <= (10 * warmupTime); ++i) {
            this_thread::sleep_until(beginTime + chrono::milliseconds(100 * i));

            // Write to trace file every 100ms
            auto nextProcessed = countProcessed();
            logWriter << 100 * i;
            logWriter << nextProcessed;
            logWriter << CsvWriter::endl;

            if ((i % 10) == 0) {
               // Write to stdout every 1s
               cerr << "WARMUP  [ " << formatTime(i / 10) << " / " << formatTime(warmupTime) << " ]: ";
               cerr << (nextProcessed - processed) << " tx/s";
               cerr << " (" << countSubmitted() << " submitted, " << nextProcessed << " processed)";
               cerr << endl;

               processed = nextProcessed;
            }
         }
      }

      if (measureTime) {
         auto beginTime = chrono::steady_clock::now();
         auto beginSubmitted = countSubmitted();
         auto beginProcessed = countProcessed();

         {
            auto processed = beginProcessed;
            for (unsigned i = 1; i <= (10 * measureTime); ++i) {
               this_thread::sleep_until(beginTime + chrono::milliseconds(100 * i));

               // Write to trace file every 100ms
               auto nextProcessed = countProcessed();
               logWriter << 100 * i;
               logWriter << nextProcessed;
               logWriter << CsvWriter::endl;

               if ((i % 10) == 0) {
                  // Write to stdout every 1s
                  cerr << "MEASURE [ " << formatTime(i / 10) << " / " << formatTime(measureTime) << " ]: ";
                  cerr << (nextProcessed - processed) << " tx/s";
                  cerr << " (" << countSubmitted() << " submitted, " << nextProcessed << " processed)";
                  cerr << endl;

                  processed = nextProcessed;
               }
            }
         }

         auto endSubmitted = countSubmitted();
         auto endProcessed = countProcessed();
         auto processed = endProcessed - beginProcessed;

         cerr << endl;
         cerr << "MEASURE THROUGHPUT: " << (static_cast<double>(processed / measureTime)) << " tx/s ";
         cerr << "(" << (endSubmitted - beginSubmitted) << " submitted, " << (endProcessed - beginProcessed) << " processed)";
         cerr << endl;
      }
   });

   // Stop processing
   monitorThread.join();

   for (auto& client : clients)
      client->stop();
   for (auto& client : clients)
      client->join();

   return true;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------