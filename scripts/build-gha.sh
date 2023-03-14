#!/bin/bash
cp scripts/stage2.sh /home/build/openwrt/stage2.sh
mkdir -p /home/build/openwrt/package/network/services/gaffa/src/src
cp scripts/Makefile /home/build/openwrt/package/network/services/gaffa/
cp CMakeLists.txt /home/build/openwrt/package/network/services/gaffa/src/
cp -r src /home/build/openwrt/package/network/services/gaffa/src/
mkdir -p /home/build/openwrt/out
bash /home/build/openwrt/stage2.sh
cp /home/build/openwrt/out/gaffa*.ipk /builds/bp2022hk1/Gaffa/
