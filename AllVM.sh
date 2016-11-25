#!/bin/sh
cwd=$(pwd)


cd "$cwd"/VM1/
vagrant $1 &

cd "$cwd"/VM2/
vagrant $1 &

cd "$cwd"/VM3/
vagrant $1 &

cd "$cwd"/VM1-6/
vagrant $1 &

cd "$cwd"/VM2-6/
vagrant $1 &

cd "$cwd"/VM3-6/
vagrant $1 &
