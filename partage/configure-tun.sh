#!/bin/sh

ip link set dev $1 up

ip addr add 172.16.2.1/28 dev $1