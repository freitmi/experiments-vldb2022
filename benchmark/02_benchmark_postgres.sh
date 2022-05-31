#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

HOST_DB_DIR=${HOST_DB_DIR:-/mnt/nvme/freitag/db/postgres}
HOST_DB_WAL_DIR=${HOST_DB_WAL_DIR:-/mnt/pmem0/freitag/db/postgres}
HOST_DATA_DIR=${HOST_DATA_DIR:-/mnt/nvme/freitag/data}

# Prepare the environment read by the individual benchmark scripts
export HOST_DB_DIR
export HOST_DB_WAL_DIR
export HOST_DATA_DIR

# Run experiments
pushd postgres || exit 1

# ----------------------------
# Scalability experiments
# ----------------------------
cores="1 2 4 8 16 24 32 40 48"

# TATP
for i in ${cores}; do
  ./benchmark-scalability-tatp.sh $i
done

# TPCC
for i in ${cores}; do
  ./benchmark-scalability-tpcc.sh $i
done

popd || exit 1