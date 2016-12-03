#!/bin/sh
ip link set dev $1 up

ip addr add $2/28 dev $1

ip route add default via $2
