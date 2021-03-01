# Host Setting
For your ubuntu (or other distributes) host, please run:
```
./init.sh
```

# Setup Docker
```
docker-compose build                         # build image
docker-compose up -d                         # create container and run it
docker ps -a                                 # view container info
docker exec -it armxcontainer /bin/bash      # get into container
docker-compose down                          # remove container
```

# Using Docker to Run ARM-X
Once you're in docker container, you can than execute following command:
```
cd /host/armx       # The /host directory is the file system outside the docker container
./run/launcher      # Run ARM-X
```

# Routing
You need to setup network routing for the connection between your ubuntu host and emulated IoT device.

```
Ubuntu <-----> Docker Container <-----> Emulated IoT Device
```
The docker container already setup NAT.

The only thing you have to do is setting following network routing for ubuntu.

```
sudo ip route add 192.168.100.0/24 via <docker container ip>
```

For example:

Docker Container Network:
```
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
2: tap0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc fq_codel state DOWN group default qlen 1000
    inet 192.168.100.1/24 scope global tap0
       valid_lft forever preferred_lft forever
5: eth0@if6: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default  link-netnsid 0
    inet 172.21.0.2/16 brd 172.21.255.255 scope global eth0
       valid_lft forever preferred_lft forever
```

Ubuntu Network:
```
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
2: ens32: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    altname enp2s0
    inet 192.168.124.141/24 brd 192.168.124.255 scope global dynamic noprefixroute ens32
       valid_lft 1251sec preferred_lft 1251sec
3: docker0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN group default
    inet 172.17.0.1/16 brd 172.17.255.255 scope global docker0
       valid_lft forever preferred_lft forever
4: br-f21015dd91d8: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default
    inet 172.21.0.1/16 brd 172.21.255.255 scope global br-f21015dd91d8
       valid_lft forever preferred_lft forever
```

Add this routing rule:
```
sudo ip route add 192.168.100.0/24 via 172.21.0.2
```
