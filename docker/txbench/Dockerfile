#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
FROM ubuntu:21.10

# Install apt dependencies
RUN apt-get update -qq \
    && DEBIAN_FRONTEND=noninteractive apt-get install tzdata \
    && apt-get install -y \
        binutils \
        ccache \
        cmake \
        g++-11 \
        make \
        ninja-build \
        lsb-release \
        gnupg \
        wget \
        gosu \
    && wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | apt-key add - \
    && sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list' \
    && apt-get update -qq \
    && apt-get install -y \
        libpq-dev \
        postgresql-server-dev-14 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 11 \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 11

WORKDIR /usr/local/src

# Build txbench binaries
COPY src/txbench txbench
RUN set -x \
    && cd txbench \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release -DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql/ .. \
    && make -j \
    && cd ../..

# Build TPCH data generator
COPY src/tpch-dbgen tpch-dbgen
RUN set -x \
    && cd tpch-dbgen \
    && make \
    && cd ..

# Build the production image
FROM ubuntu:21.10

# Install apt dependencies
RUN apt-get update -qq \
    && DEBIAN_FRONTEND=noninteractive apt-get install tzdata \
    && apt-get install -y \
        lsb-release \
        gnupg \
        wget \
        gosu \
    && wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | apt-key add - \
    && sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list' \
    && apt-get update -qq \
    && apt-get install -y \
        libpq5 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Copy binaries
COPY --from=0 /usr/local/src/txbench/build/tools/postgres/txbench-postgres /usr/local/bin/txbench-postgres
COPY --from=0 /usr/local/src/txbench/build/tools/umbra/txbench-umbra /usr/local/bin/txbench-umbra
COPY --from=0 /usr/local/src/txbench/build/tools/datagen/txbench-datagen /usr/local/bin/txbench-datagen
COPY --from=0 /usr/local/src/tpch-dbgen/dbgen /usr/local/bin/tpch-dbgen
COPY --from=0 /usr/local/src/tpch-dbgen/dists.dss /var/lib/txbench/dists.dss
COPY tpch-dbgen.sh /var/lib/txbench/tpch-dbgen.sh

# Setup entrypoint
COPY entrypoint.sh /entrypoint.sh
RUN set -x \
    && chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]

# Client working directory
WORKDIR /var/lib/txbench
