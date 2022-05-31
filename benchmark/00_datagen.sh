#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

HOST_DATA_DIR=${HOST_DATA_DIR:-/mnt/nvme/freitag/data}

# Prepare the environment read by the individual datagen scripts
export HOST_DATA_DIR

pushd datagen || exit 1

./datagen-tatp.sh
./datagen-tpcc.sh
./datagen-tpch.sh

popd || exit 1
