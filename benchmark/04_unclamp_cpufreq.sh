#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

# Go through sudo if required
if [ $EUID != 0 ]; then
  sudo "$0" "$@"
  exit $?
fi

# Select performance governor and reset upper frequency limit to maximum supported by hardware
for ((i = 0; i < $(nproc); i++)); do
  max_freq=$(cpufreq-info -c $i -l | cut -d' ' -f2)

  cpufreq-set -c $i -g performance
  cpufreq-set -c $i -u ${max_freq}
done
