FROM ubuntu:focal
MAINTAINER ljptw

RUN apt-get update
RUN apt-get -y install rpcbind nfs-common nfs-kernel-server
RUN apt-get -y install uml-utilities dialog
RUN apt-get -y install zip unzip
RUN apt-get -y install iproute2 iptables iputils-ping
RUN apt-get -y install qemu-system

# Setting: NFS
RUN echo "/host/armx *(rw,sync,no_subtree_check)" >> /etc/exports

# Setting: Initialization
RUN echo "./host/armx/init.sh" >> ~/.bashrc

ENTRYPOINT ["/bin/bash"]
