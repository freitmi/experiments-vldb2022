#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

scale_factor=${1:-1}

# Generate initial data population
DSS_PATH=/data/tpch/initial tpch-dbgen -vf -s ${scale_factor}

# Generate updates (a full cycle consists of 4000 update/delete pairs but our benchmarks do not run that long)
DSS_PATH=/data/tpch tpch-dbgen -vf -s ${scale_factor} -U 1000
mv /data/tpch/delete.* /data/tpch/delete
mv /data/tpch/*.tbl.u* /data/tpch/update