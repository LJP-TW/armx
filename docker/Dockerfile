FROM ubuntu:focal
MAINTAINER ljptw

RUN apt-get update
RUN apt-get -y install rpcbind nfs-common nfs-kernel-server
RUN apt-get -y install uml-utilities dialog
RUN apt-get -y install zip unzip
RUN apt-get -y install iproute2 iptables iputils-ping
RUN apt-get -y install qemu-system
RUN apt-get -y install wget make gcc
RUN apt-get -y install g++ patch cpio rsync bc perl

# Build buildroot
WORKDIR /root
RUN wget https://buildroot.org/downloads/buildroot-2020.02.9.tar.gz
RUN tar -zxvf buildroot-2020.02.9.tar.gz
WORKDIR /root/buildroot-2020.02.9
RUN wget https://gist.githubusercontent.com/LJP-TW/f17a11131191aba810bf7dd4b98e628a/raw/7eca45e411d0b269f4fb7bb93b936871359459ae/buildroot_toolchain.config -O .config

ENV FORCE_UNSAFE_CONFIGURE=1
RUN make toolchain

# Setting: NFS
RUN echo "/host/armx *(rw,sync,no_subtree_check)" >> /etc/exports

# Setting: Initialization
RUN echo "./host/armx/init.sh" >> ~/.bashrc

WORKDIR /
ENTRYPOINT ["/bin/bash"]
