find_package(PostgreSQL 14)

set(LIB_TXBENCH_BASE_SOURCES
        txbench/io/CsvWriter.cpp
        txbench/io/JsonParser.cpp
        txbench/io/ProgressPrinter.cpp
        txbench/tatp/Generator.cpp
        txbench/tatp/TransactionGenerator.cpp
        txbench/tpcc/Generator.cpp
        txbench/tpcc/TransactionGenerator.cpp
        )

set(LIB_TXBENCH_PQ_SOURCES
        txbench/pq/AsyncCall.cpp
        txbench/pq/Connection.cpp
        txbench/pq/ConnectionString.cpp
        txbench/pq/PreparedStatement.cpp
        txbench/pq/Result.cpp
        txbench/pq/ResultSet.cpp
        )

add_library(txbench-base STATIC ${LIB_TXBENCH_BASE_SOURCES})
target_include_directories(txbench-base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(txbench-base PRIVATE picojson)

if (${PostgreSQL_FOUND})
    add_library(txbench-pq STATIC ${LIB_TXBENCH_PQ_SOURCES})
    target_include_directories(txbench-pq PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(txbench-pq PUBLIC txbench-base)
    target_link_libraries(txbench-pq PRIVATE PostgreSQL::PostgreSQL)
endif ()
