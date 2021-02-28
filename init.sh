#!/usr/bin/env bash

echo 'executing init.sh ...'

ARMX_DIR=$(dirname "$0")
./${ARMX_DIR}/init_ip.sh
./${ARMX_DIR}/init_nfs.sh

if [ ! -f ${ARMX_DIR}/hostfs/hostfs.ext2 ]; then
    unzip -p ${ARMX_DIR}/hostfs/hostfs.ext2.zip > ${ARMX_DIR}/hostfs/hostfs.ext2
fi

