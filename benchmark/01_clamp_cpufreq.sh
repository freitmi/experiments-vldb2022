#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

# Go through sudo if required
if [ $EUID != 0 ]; then
  sudo "$0" "$@"
  exit $?
fi

# Select performance governor and set upper frequency limit to 2.6GHz
for ((i = 0; i < $(nproc); i++)); do
  cpufreq-set -c $i -g performance
  cpufreq-set -c $i -u 2.6GHz
done
