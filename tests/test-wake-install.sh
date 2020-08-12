#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(dirname $0)

OUTPUT_PATH="build/freedom-metal/install"

wake --init .

wake -v --in freedom_metal -x "installFreedomMetal \"${OUTPUT_PATH}\""

OUTPUTS=("${OUTPUT_PATH}/freedom-metal/LICENSE" )
for file in ${OUTPUTS[@]}; do
        >&2 echo "$0: Checking for ${file}"
        if [ ! -f ${file} ] ; then
                >&2 echo "$0: ERROR Failed to produce ${file}"
                exit 1
        fi
done
