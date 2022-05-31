#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

HOST_UID=$(id -u "${USER}")
HOST_GID=$(id -g "${USER}")

HOST_DATA_DIR=${HOST_DATA_DIR:-/mnt/nvme/freitag/data}

# Create the data directory
mkdir -p "${HOST_DATA_DIR}/tpch/initial"
mkdir -p "${HOST_DATA_DIR}/tpch/delete"
mkdir -p "${HOST_DATA_DIR}/tpch/update"

# Prepare the environment read by the docker-compose file
export HOST_UID
export HOST_GID
export HOST_DATA_DIR

# Generate data
docker-compose run --rm txbench-client ./tpch-dbgen.sh 10
docker-compose down --remove-orphans
