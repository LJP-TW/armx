#!/usr/bin/env bash

echo 'executing fix.sh ...'

HOSTFS_DIR=$(dirname "$0")

e2fsck ${HOSTFS_DIR}/hostfs.ext2
