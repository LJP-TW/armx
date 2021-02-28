#!/usr/bin/env bash

# Host needs to install nfs-kernel-server
#   sudo apt-get install nfs-kernel-server

echo 'executing init_nfs.sh ...'

update-rc.d rpcbind enable
update-rc.d nfs-common enable
service rpcbind start
service nfs-kernel-server start

