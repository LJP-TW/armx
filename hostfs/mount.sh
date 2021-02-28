#!/usr/bin/env bash

echo 'executing mount.sh ...'

HOSTFS_DIR=$(dirname "$0")
if [ ! -d "/mnt/armx" ]; then
    mkdir -p /mnt/armx
fi

if [ "$(ls -A /mnt/armx)" ]; then
    echo "/mnt/armx isn't empty..."
else
    mount -o rw,loop ${HOSTFS_DIR}/hostfs.ext2 /mnt/armx
fi
