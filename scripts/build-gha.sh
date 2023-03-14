#!/bin/bash
set -xe
mkdir -p out package/src/src
cp scripts/Makefile package/
cp CMakeLists.txt package/src/
cp -r src package/src/

docker run --rm \
  -v "$(pwd)"/scripts/stage2.sh:/home/build/openwrt/stage2.sh \
  -v "$(pwd)"/package/:/home/build/openwrt/package/network/services/gaffa \
  -v "$(pwd)"/out/:/home/build/openwrt/out\
  wimove-buildenv/arm:$TAG \
  bash stage2.sh