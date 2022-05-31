#!/bin/bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

# Build docker image
docker build --tag=ghcr.io/freitmi/txbench-umbra --ulimit nofile=2048 .
