#!/bin/sh
set -ex
shopt -s globstar
cd /home/build/openwrt
make "-j$(nproc)" V=sc package/network/services/gaffa/compile
cp bin/targets/**/packages/gaffa*.ipk out
