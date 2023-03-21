#!/bin/sh
set -ex
shopt -s globstar
ls -lisa /home/build/openwrt/package/network/services/
cd /home/build/openwrt
make V=sc package/network/services/gaffa/compile
cp bin/targets/**/packages/gaffa*.ipk out
