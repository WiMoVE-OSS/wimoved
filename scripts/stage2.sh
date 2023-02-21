#!/bin/sh
set -ex
cd /home/build/openwrt
make "-j$(nproc)" V=sc package/network/services/gaffa/compile
cp bin/targets/mvebu/cortexa9/packages/gaffa*.ipk out
