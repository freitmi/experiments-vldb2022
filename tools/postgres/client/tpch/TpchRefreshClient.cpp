#include "client/tpch/TpchRefreshClient.hpp"
#include "txbench/pq/ResultSet.hpp"
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
TpchRefreshClient::TpchRefreshClient(const ClientConfig& clientConfig, const TpchConfig& tpchConfig)
   : Client(clientConfig), tpchConfig(tpchConfig)
// Constructor
{
}
//---------------------------------------------------------------------------
void TpchRefreshClient::performWork()
// Perform work
{
   unsigned index = 0;
   while (!done.load(memory_order_acquire)) {
      // Insert new sales
      connection.executeCommand("BEGIN");
      if (auto res = connection.executeCommand("COPY orders FROM PROGRAM 'sed s/.$// /data/tpch/update/orders.tbl.u" + to_string(index + 1) + "' DELIMITER '|'").getStatus(); !res) {
         cerr << "unable to execute command: " << res.getMessage() << endl;
         break;
      }
      if (auto res = connection.executeCommand("COPY lineitem FROM PROGRAM 'sed s/.$// /data/tpch/update/lineitem.tbl.u" + to_string(index + 1) + "' DELIMITER '|'").getStatus(); !res) {
         cerr << "unable to execute command: " << res.getMessage() << endl;
         break;
      }
      connection.executeCommand("COMMIT");

      // Remove old sales
      connection.executeCommand("BEGIN");
      if (auto res = connection.executeCommand("CREATE TEMPORARY TABLE to_delete (id INTEGER NOT NULL) ON COMMIT DROP").getStatus(); !res) {
         cerr << "unable to execute command: " << res.getMessage() << endl;
         break;
      }
      if (auto res = connection.executeCommand("COPY to_delete FROM PROGRAM 'sed s/.$// /data/tpch/delete/delete." + to_string(index + 1) + "' DELIMITER '|'").getStatus(); !res) {
         cerr << "unable to execute command: " << res.getMessage() << endl;
         break;
      }
      if (auto res = connection.executeCommand("DELETE FROM orders USING to_delete WHERE o_orderkey = id;").getStatus(); !res) {
         cerr << "unable to execute command: " << res.getMessage() << endl;
         break;
      }
      if (auto res = connection.executeCommand("DELETE FROM lineitem USING to_delete WHERE l_orderkey = id;").getStatus(); !res) {
         cerr << "unable to execute command: " << res.getMessage() << endl;
         break;
      }
      connection.executeCommand("COMMIT");

      // There are exactly 4000 update / delete pairs before data returns to the initial state
      index = ((index + 1) % 4000);
   }
}
//---------------------------------------------------------------------------
Status TpchRefreshClient::initialize()
// Initialize the client
{
   if (auto res = Client::initialize(); !res)
      return res;

   return Status::Success;
}
//---------------------------------------------------------------------------
void TpchRefreshClient::start()
// Start processing
{
   done.store(false, memory_order_release);
   thread = ::thread([this]() { performWork(); });
}
//---------------------------------------------------------------------------
void TpchRefreshClient::stop()
// Stop processing
{
   done.store(true, memory_order_release);
}
//---------------------------------------------------------------------------
void TpchRefreshClient::join()
// Join the client
{
   thread.join();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------