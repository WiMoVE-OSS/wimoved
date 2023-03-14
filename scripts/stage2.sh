#!/bin/sh
set -ex
shopt -s globstar
cd /home/build/openwrt
make "-j$(nproc)" V=sc package/network/services/wimove/compile
cp bin/targets/**/packages/wimove*.ipk out
