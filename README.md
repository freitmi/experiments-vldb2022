# VLDB 2022 Benchmark Suite

This repository contains the benchmark suite used to generate the results reported in our paper 
"Memory-Optimized Multi-Version Concurrency Control for Disk-Based Database Systems". We provide 
step-by-step instructions on how to reproduce our results below.

Copyright (c) 2022 TUM. All rights reserved.

## Dependencies

The actual benchmark code is encapsulated within multiple Docker containers that are orchestrated using Docker Compose. 
We host pre-built Docker images for convenience, i.e. there is no need to manually build Docker containers. The `docker` 
subdirectory contains the build scripts used to generate the Docker images. In order to run the benchmarks, the 
following dependencies are required.

- bash
- cpufrequtils (optional, for CPU frequency clamping)
- Docker
- Docker Compose

## Data generation

Before running any benchmarks, the respective data sets have to be generated once. This can be achieved as follows, 
assuming that the current working directory contains this README file.

1. `cd benchmark`
2. `HOST_DATA_DIR=<DATA_DIR> ./00_datagen.sh`

The environment variable `HOST_DATA_DIR` should point to the base directory into which the data sets will be placed. 
After successful data generation, this directory should contain the following subdirectories:

```
${HOST_DATA_DIR}
├─── tatp
|    └─── ...               Multiple .csv files 
├─── tpcc
|    └─── ...               Multiple .csv files 
└─── tpch
     ├─── delete
     |    └─── ...          Multiple delete.N files 
     ├─── initial
     |    └─── ...          Multiple .tbl files
     └─── update
          └─── ...          Multiple lineitem.tbl.uN and orders.tbl.uN files
```

## Running benchmarks

Once data has been generated, the benchmarks can be run as follows. Once again, the listed commands assume that the 
working directory contains this README file.

1. `cd benchmark`
2. `./01_clamp_cpufreq.sh` (optional; sets the CPU frequency governor to "performance" and limits frequency scaling to 2.6 GHz)
4. `HOST_DB_DIR=<DB_DIR> HOST_DB_WAL_DIR=<DB_WAL_DIR> HOST_DATA_DIR=<DATA_DIR> ./02_benchmark_postgres.sh`
5. `HOST_DB_DIR=<DB_DIR> HOST_DB_WAL_DIR=<DB_WAL_DIR> HOST_DATA_DIR=<DATA_DIR> ./03_benchmark_umbra.sh`
6. `./04_unclamp_cpufreq.sh` (optional; sets the CPU frequency governor to "performance" and resets frequency limits to maximum)

The following environment variables influence the behavior of the benchmark scripts:

- `HOST_DB_DIR`: The directory into which the database files should be placed
- `HOST_DB_WAL_DIR`: The directory into which the database write-ahead log should be placed
- `HOST_DATA_DIR`: The directory which contains the data sets generated in the previous step

Benchmark results will be placed into the directories `postgres/output`, or `umbra/output` for the respective system.