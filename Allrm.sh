#!/bin/sh
cwd=$(pwd)


cd "$cwd"/VM1/
rm -r .vagrant/

cd "$cwd"/VM2/
rm -r .vagrant/

cd "$cwd"/VM3/
rm -r .vagrant/

cd "$cwd"/VM1-6/
rm -r .vagrant/

cd "$cwd"/VM2-6/
rm -r .vagrant/

cd "$cwd"/VM3-6/
rm -r .vagrant/
