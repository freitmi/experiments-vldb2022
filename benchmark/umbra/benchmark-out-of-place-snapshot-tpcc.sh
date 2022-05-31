#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

HOST_UID=$(id -u "${USER}")
HOST_GID=$(id -g "${USER}")

HOST_DB_DIR=${HOST_DB_DIR:-/mnt/nvme/freitag/db/umbra}
HOST_DB_WAL_DIR=${HOST_DB_WAL_DIR:-/mnt/pmem0/freitag/db/umbra}
HOST_DATA_DIR=${HOST_DATA_DIR:-/mnt/nvme/freitag/data}

HOST_CONFIG_DIR=${HOST_CONFIG_DIR:-$(pwd)/txbench/scripts/tpcc/config_out_of_place_snapshot}
HOST_SCRIPT_DIR=${HOST_SCRIPT_DIR:-$(pwd)/txbench/scripts/tpcc/sql}
HOST_OUTPUT_DIR=${HOST_OUTPUT_DIR:-$(pwd)/output/out-of-place-snapshot-tpcc}

# Clear the database directory
rm -rf "${HOST_DB_DIR}"
rm -rf "${HOST_DB_WAL_DIR}"
mkdir -p "${HOST_DB_DIR}"
mkdir -p "${HOST_DB_WAL_DIR}"

# Create the output directory
mkdir -p "${HOST_OUTPUT_DIR}"

# Prepare the environment read by the docker-compose file
export HOST_UID
export HOST_GID
export HOST_DB_DIR
export HOST_DB_WAL_DIR
export HOST_DATA_DIR
export HOST_CONFIG_DIR
export HOST_SCRIPT_DIR
export HOST_OUTPUT_DIR

# Run the benchmark
docker-compose run --rm txbench-client txbench-umbra /config/client-config.json TPCC /config/tpcc-config.json $1 /output/client_trace_$1.csv
docker-compose down --remove-orphans

# Clear the database directory
rm -rf "${HOST_DB_DIR}"
rm -rf "${HOST_DB_WAL_DIR}"
