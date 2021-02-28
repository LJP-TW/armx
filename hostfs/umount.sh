#!/usr/bin/env bash

echo 'executing umount.sh ...'

if [ "$(ls -A /mnt/armx)" ]; then
    umount /mnt/armx
else
    echo "/mnt/armx is empty..."
fi
