#!/bin/sh
set -ex
shopt -s globstar
cd /home/build/openwrt
make "-j$(nproc)" V=sc prereq
make "-j$(nproc)" V=sc package/network/services/wimoved/compile
cp bin/targets/**/packages/wimoved*.ipk out
