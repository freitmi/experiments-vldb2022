#ifndef H_tools_postgres_client_Driver
#define H_tools_postgres_client_Driver
//---------------------------------------------------------------------------
#include "client/ClientConfig.hpp"
#include "client/DriverConfig.hpp"
#include <memory>
#include <vector>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
class Client;
//---------------------------------------------------------------------------
/// A generic benchmark driver implementation
class Driver {
   protected:
   /// The client configuration
   const ClientConfig& clientConfig;
   /// The driver configuration
   const DriverConfig& driverConfig;
   /// The number of clients
   unsigned clientCount;
   /// The logfile
   std::string logfile;

   /// The clients
   std::vector<std::unique_ptr<Client>> clients;

   /// Count the number of submitted transactions
   uint64_t countSubmitted();
   /// Count the number of processed transactions
   uint64_t countProcessed();

   /// Create a client
   virtual std::unique_ptr<Client> createClient(unsigned clientIndex) const = 0;

   public:
   /// Constructor
   Driver(const ClientConfig& clientConfig, const DriverConfig& driverConfig, unsigned clientCount, std::string logfile);
   /// Destructor
   virtual ~Driver();

   /// Run the driver
   bool run();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif