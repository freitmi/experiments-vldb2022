#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
FROM ghcr.io/freitmi/umbra-release:80e986c9d

# Setup entrypoint
COPY entrypoint.sh /entrypoint.sh
COPY docker-entrypoint.sh /docker-entrypoint.sh
RUN set -x \
    && chmod +x /entrypoint.sh \
    && chmod +x /docker-entrypoint.sh
ENTRYPOINT ["/entrypoint.sh", "/docker-entrypoint.sh"]
