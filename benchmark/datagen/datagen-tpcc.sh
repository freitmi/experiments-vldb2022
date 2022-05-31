#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

HOST_UID=$(id -u "${USER}")
HOST_GID=$(id -g "${USER}")

HOST_DATA_DIR=${HOST_DATA_DIR:-/mnt/nvme/freitag/data}

# Create the data directory
mkdir -p "${HOST_DATA_DIR}/tpcc"

# Prepare the environment read by the docker-compose file
export HOST_UID
export HOST_GID
export HOST_DATA_DIR

# Generate data
docker-compose run --rm txbench-client txbench-datagen TPCC 100 /data/tpcc
docker-compose down --remove-orphans
