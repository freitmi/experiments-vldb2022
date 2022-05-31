#ifndef H_tools_postgres_client_Client
#define H_tools_postgres_client_Client
//---------------------------------------------------------------------------
#include "client/ClientConfig.hpp"
#include "txbench/Status.hpp"
#include "txbench/pq/Connection.hpp"
#include <atomic>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// A client
class Client {
   protected:
   /// The client configuration
   const ClientConfig& clientConfig;
   /// The connection
   pq::Connection connection;

   /// The number of submitted transactions
   std::atomic<uint64_t> submitted = 0;
   /// The number of processed transactions
   std::atomic<uint64_t> processed = 0;

   public:
   /// Constructor
   explicit Client(const ClientConfig& clientConfig);
   /// Destructor
   virtual ~Client();

   /// Get the number of submitted transactions
   uint64_t getSubmitted() const;
   /// Get the number of processed transactions
   uint64_t getProcessed() const;

   /// Initialize the client
   virtual Status initialize();

   /// Start processing
   virtual void start() = 0;
   /// Stop processing
   virtual void stop() = 0;
   /// Wait for completion
   virtual void join() = 0;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif