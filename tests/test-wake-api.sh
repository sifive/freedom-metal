#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(dirname $0)
SPIKE_DTS_DIR=${SCRIPT_DIR}/spike

OUTPUT_PATH="build/freedom-metal/generated"

wake --init .

wake -v --in freedom_metal -x "runFreedomMetalCodeGeneration \"build\" (makeFreedomMetalCodeGenerationOptions (source \"${SPIKE_DTS_DIR}/design.dts\") (sources \"${SPIKE_DTS_DIR}\" \`core.dts\`) \"${OUTPUT_PATH}\")"

OUTPUTS=("${OUTPUT_PATH}/metal.mk" )
for file in ${OUTPUTS[@]}; do
        >&2 echo "$0: Checking for ${file}"
        if [ ! -f ${file} ] ; then
                >&2 echo "$0: ERROR Failed to produce ${file}"
                exit 1
        fi
done
