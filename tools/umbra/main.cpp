#include "client/ClientConfig.hpp"
#include "client/Driver.hpp"
#include "client/load/LoadConfig.hpp"
#include "client/tatp/TatpConfig.hpp"
#include "client/tatp/TatpDriver.hpp"
#include "client/tpcc/TpccConfig.hpp"
#include "client/tpcc/TpccDriver.hpp"
#include "client/tpch/TpchConfig.hpp"
#include "client/tpch/TpchDriver.hpp"
#include "txbench/pq/Connection.hpp"
#include "txbench/pq/ConnectionString.hpp"
#include "txbench/pq/Result.hpp"
#include <charconv>
#include <chrono>
#include <fstream>
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
static Status readScript(const string& filename, string& target)
// Read a query script from a file
{
   ifstream t(filename);
   if (!t.is_open())
      return Status::Error("unable to open \"" + filename + "\"");

   target = string((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
   return Status::Success;
}
//---------------------------------------------------------------------------
static Status executeScript(pq::Connection& connection, const string& filename)
// Execute a script
{
   string script;
   if (auto res = readScript(filename, script); !res)
      return res;
   if (auto res = connection.executeCommand(script).getStatus(); !res)
      return res;

   return Status::Success;
}
//---------------------------------------------------------------------------
static int main(int argc, const char* argv[])
// Entry point
{
   if (argc != 6) {
      cerr << "USAGE: " << argv[0] << " CLIENT_CONFIG (TPCC|TATP|TPCH|LOAD) BENCHMARK_CONFIG CLIENT_COUNT LOGFILE" << endl;
      return 1;
   }

   ClientConfig clientConfig;
   if (auto res = clientConfig.read(argv[1]); !res) {
      cerr << "unable to open client config at \"" << argv[1] << "\": " << res.getMessage() << endl;
      return 1;
   }

   string_view clientCountString = argv[4];
   unsigned clientCount;
   auto [ptr, ec] = from_chars(clientCountString.data(), clientCountString.data() + clientCountString.size(), clientCount);
   if ((ec != std::errc()) || (ptr != clientCountString.data() + clientCountString.size())) {
      cerr << "unable to parse client count \"" << clientCountString << "\"" << endl;
      return 1;
   }

   string benchmark = argv[2];
   if (benchmark == "LOAD") {
      LoadConfig loadConfig;
      if (auto res = loadConfig.read(argv[3]); !res) {
         cerr << "unable to open loadtest config at \"" << argv[3] << "\": " << res.getMessage() << endl;
         return 1;
      }

      // Setup schema and load benchmark data
      pq::Connection connection;
      if (auto res = connection.connect(clientConfig.getConnectionString()); !res) {
         cerr << "unable to connect: " << res.getMessage() << endl;
         return 1;
      }

      cerr << "setting up schema..." << endl;
      for (const auto& filename : clientConfig.getSchemaScripts()) {
         if (auto res = executeScript(connection, filename); !res) {
            cerr << "unable to execute schema script \"" << filename << "\":" << res.getMessage() << endl;
            return 1;
         }
      }

      // Start server-side tracing
      if (auto res = connection.executeCommand("ALTER SYSTEM SET tracing.filename = '/output/server_trace_" + to_string(clientCount) + ".csv'").getStatus(); !res) {
         cerr << "unable to configure server-side tracing: " << res.getMessage() << endl;
         return 1;
      }

      if (auto res = connection.executeCommand("ALTER SYSTEM SET tracing = on").getStatus(); !res) {
         cerr << "unable to start server-side tracing: " << res.getMessage() << endl;
         return 1;
      }

      cerr << "loading data..." << endl;

      auto begin = chrono::steady_clock::now();
      if (loadConfig.useBulkOperations()) {
         if (auto res = connection.executeCommand("BEGIN BULK WRITE").getStatus(); !res) {
            cerr << "unable to execute command: " << res.getMessage() << endl;
            return 1;
         }
      } else {
         if (auto res = connection.executeCommand("BEGIN").getStatus(); !res) {
            cerr << "unable to execute command: " << res.getMessage() << endl;
            return 1;
         }
      }

      for (const auto& filename : clientConfig.getLoadScripts()) {
         if (auto res = executeScript(connection, filename); !res) {
            cerr << "unable to execute load script \"" << filename << "\":" << res.getMessage() << endl;
            return 1;
         }
      }

      if (auto res = connection.executeCommand("COMMIT").getStatus(); !res) {
         cerr << "unable to execute command: " << res.getMessage() << endl;
         return 1;
      }
      auto end = chrono::steady_clock::now();

      // Stop server-side tracing
      if (auto res = connection.executeCommand("ALTER SYSTEM SET tracing = off").getStatus(); !res) {
         cerr << "unable to stop server-side tracing: " << res.getMessage() << endl;
         return 1;
      }

      // Write client-side trace file
      {
         ofstream out("/output/client_trace_" + to_string(clientCount) + ".csv");
         out << (1e-9 * (end - begin).count()) << endl;
      }

      return 0;
   } else {
      {
         // Setup schema and load benchmark data
         pq::Connection connection;
         if (auto res = connection.connect(clientConfig.getConnectionString()); !res) {
            cerr << "unable to connect: " << res.getMessage() << endl;
            return 1;
         }

         cerr << "setting up schema..." << endl;
         for (const auto& filename : clientConfig.getSchemaScripts()) {
            if (auto res = executeScript(connection, filename); !res) {
               cerr << "unable to execute schema script \"" << filename << "\":" << res.getMessage() << endl;
               return 1;
            }
         }

         cerr << "loading data..." << endl;
         for (const auto& filename : clientConfig.getLoadScripts()) {
            if (auto res = executeScript(connection, filename); !res) {
               cerr << "unable to execute load script \"" << filename << "\":" << res.getMessage() << endl;
               return 1;
            }
         }
      }

      // Setup the benchmark driver
      unique_ptr<Driver> driver;

      string logfile = argv[5];
      if (benchmark == "TPCC") {
         TpccConfig tpccConfig;
         if (auto res = tpccConfig.read(argv[3]); !res) {
            cerr << "unable to open TPCC config at \"" << argv[3] << "\": " << res.getMessage() << endl;
            return 1;
         }

         driver = make_unique<TpccDriver>(clientConfig, tpccConfig, clientCount, move(logfile));
      } else if (benchmark == "TATP") {
         TatpConfig tatpConfig;
         if (auto res = tatpConfig.read(argv[3]); !res) {
            cerr << "unable to open TATP config at \"" << argv[3] << "\": " << res.getMessage() << endl;
            return 1;
         }

         driver = make_unique<TatpDriver>(clientConfig, tatpConfig, clientCount, move(logfile));
      } else if (benchmark == "TPCH") {
         TpchConfig tpchConfig;
         if (auto res = tpchConfig.read(argv[3]); !res) {
            cerr << "unable to open TPCH config at \"" << argv[3] << "\": " << res.getMessage() << endl;
            return 1;
         }

         driver = make_unique<TpchDriver>(clientConfig, tpchConfig, clientCount, move(logfile));
      } else {
         cerr << "unknown benchmark \"" << benchmark << "\"";
         return 1;
      }

      return driver->run() ? 0 : 1;
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
int main(int argc, const char* argv[])
// Entry point
{
   return txbench::umbra::main(argc, argv);
}
//---------------------------------------------------------------------------