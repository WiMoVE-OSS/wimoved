#!/bin/sh
set -ex
shopt -s globstar
cd /home/build/openwrt
while [ ! -f "package/network/services/gaffa/Makefile" ]; do
  echo "waiting for package/network/services/gaffa"
  sleep 1
done
make "-j$(nproc)" V=sc package/network/services/gaffa/compile
cp bin/targets/**/packages/gaffa*.ipk out
