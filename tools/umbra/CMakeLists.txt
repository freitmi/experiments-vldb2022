set(TXBENCH_UMBRA_SOURCES
        client/load/LoadConfig.cpp
        client/tatp/TatpClient.cpp
        client/tatp/TatpConfig.cpp
        client/tatp/TatpDriver.cpp
        client/tpcc/TpccClient.cpp
        client/tpcc/TpccConfig.cpp
        client/tpcc/TpccDriver.cpp
        client/tpch/TpchConfig.cpp
        client/tpch/TpchDriver.cpp
        client/tpch/TpchQueryClient.cpp
        client/tpch/TpchRefreshClient.cpp
        client/Client.cpp
        client/ClientConfig.cpp
        client/Driver.cpp
        main.cpp
        )

add_executable(txbench-umbra ${TXBENCH_UMBRA_SOURCES})
target_include_directories(txbench-umbra PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(txbench-umbra txbench-pq)