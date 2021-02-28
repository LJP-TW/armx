#!/usr/bin/env bash

echo 'executing init_ip.sh ...'

tunctl -t tap0 -u root
ip link set tap0 up
ip addr add 192.168.100.1/24 dev tap0

iptables -t nat -A POSTROUTING -o tap0 -j MASQUERADE
iptables -A FORWARD -i eth0 -o tap0 -m state --state RELATED,ESTABLISHED -j ACCEPT
iptables -A FORWARD -i eth0 -o tap0 -j ACCEPT

