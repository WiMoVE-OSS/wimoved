#!/bin/bash
set -xe
rm -r out package || true
mkdir -p out package/src/src
chmod -R ogu+rw out
cp scripts/Makefile package/
cp CMakeLists.txt package/src/
cp -r src package/src/

docker run --rm \
  -v "$(pwd)"/scripts/stage2.sh:/home/build/openwrt/stage2.sh \
  -v "$(pwd)"/package:/home/build/openwrt/package/network/services/wimoved \
  -v "$(pwd)"/out:/home/build/openwrt/out \
  $IMAGE \
  bash stage2.sh
