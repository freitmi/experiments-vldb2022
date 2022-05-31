#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

# Setup a database
umbra-sql -createdb /db <<< "ALTER ROLE postgres WITH LOGIN SUPERUSER PASSWORD '${POSTGRES_PASSWORD}';" || exit 1

# Source configuration
source /config/umbra-env.sh || exit 1

# Start the server
umbra-server -address 0.0.0.0 -quiet -connectionHandlers 48 /db || exit 1