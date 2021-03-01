#!/bin/sh

AC15_DIR=$(dirname "$0")

# Copy kernel
sudo cp ${AC15_DIR}/../template/kernel/zImage-2.6.39.4-vexpress ${AC15_DIR}/kernel

# Make rootfs
sudo mkdir ${AC15_DIR}/rootfs
cd ${AC15_DIR}/rootfs
sudo cpio -idm < ../rootfs.cpio
