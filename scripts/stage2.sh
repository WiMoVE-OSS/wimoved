#!/bin/sh
set -ex
shopt -s globstar
ls -lisa /home/build/openwrt/package/network/services/
cd /home/build/openwrt
make "-j$(nproc)" V=sc prereq
make "-j$(nproc)" V=sc package/network/services/gaffa/compile
cp bin/targets/**/packages/gaffa*.ipk out
