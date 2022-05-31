#!/bin/bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

TXBENCH_ROOT=../..

# Copy source tree
mkdir -p src

if [ -d src/txbench ]; then
  rm -rf src/txbench
fi

mkdir -p src/txbench

cp -r ${TXBENCH_ROOT}/lib src/txbench
cp -r ${TXBENCH_ROOT}/thirdparty src/txbench
cp -r ${TXBENCH_ROOT}/tools/ src/txbench
cp ${TXBENCH_ROOT}/CMakeLists.txt src/txbench

# Clone TPCH data generator
if [ ! -d src/tpch-dbgen ]; then
  git clone https://github.com/electrum/tpch-dbgen.git src/tpch-dbgen
fi

# Build docker image
docker build --tag=ghcr.io/freitmi/txbench-client --ulimit nofile=2048 .
