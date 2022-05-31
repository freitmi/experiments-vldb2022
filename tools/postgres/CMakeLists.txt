set(TXBENCH_POSTGRES_SOURCES
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

add_executable(txbench-postgres ${TXBENCH_POSTGRES_SOURCES})
target_include_directories(txbench-postgres PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(txbench-postgres txbench-pq)