#!/bin/bash
set -xe
rm -r out package || true
mkdir -p out package/src/src
cp scripts/Makefile package/
cp CMakeLists.txt package/src/
cp -r src package/src/
cp -r vendor package/src/

docker build "$(pwd)/$(dirname -- "$0")" -t wimoved-buildenv

docker run --rm \
  -v "$(pwd)"/scripts/stage2.sh:/home/build/openwrt/stage2.sh \
  -v "$(pwd)"/package/:/home/build/openwrt/package/network/services/wimoved \
  -v "$(pwd)"/out/:/home/build/openwrt/out wimoved-buildenv \
  bash stage2.sh
