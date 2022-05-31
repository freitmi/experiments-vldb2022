#ifndef H_tools_postgres_client_tatp_TatpClient
#define H_tools_postgres_client_tatp_TatpClient
//---------------------------------------------------------------------------
#include "client/Client.hpp"
#include "client/tatp/TatpConfig.hpp"
#include "txbench/pq/PreparedStatement.hpp"
#include "txbench/tatp/TransactionGenerator.hpp"
#include <atomic>
#include <thread>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// TATP client
class TatpClient : public Client {
   private:
   /// The TATP configuration
   const TatpConfig& tatpConfig;

   /// The processing thread
   std::thread thread;
   /// Whether we are done
   std::atomic<bool> done;

   /// The transaction generator
   tatp::TransactionGenerator generator;
   /// The subscriber count
   unsigned subscriberCount;

   /// The get subscriber data statement
   pq::PreparedStatement getSubscriberData;
   /// The get new destination statement
   pq::PreparedStatement getNewDestination;
   /// The get access data statement
   pq::PreparedStatement getAccessData;
   /// The update subscriber data statement
   pq::PreparedStatement updateSubscriberData;
   /// The update location statement
   pq::PreparedStatement updateLocation;
   /// The insert call forwarding statement
   pq::PreparedStatement insertCallForwarding;
   /// The delete call forwarding statement
   pq::PreparedStatement deleteCallForwarding;

   /// Perform work
   void performWork();

   public:
   /// Constructor
   explicit TatpClient(const ClientConfig& clientConfig, const TatpConfig& tatpConfig, unsigned clientIndex);

   /// Initialize the client
   Status initialize() override;

   /// Start processing
   void start() override;
   /// Stop processing
   void stop() override;
   /// Join the client
   void join() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif