#!/bin/bash
set -xe
rm -r out package || true
mkdir -p out package/src/src
chmod -R ogu+rw out
cp scripts/openwrtfiles/* package/
cp CMakeLists.txt package/
cp -r src package/
cp -r vendor package/

docker run --rm \
  -v "$(pwd)"/package:/home/build/openwrt/package/network/services/wimoved \
  -v "$(pwd)"/out:/home/build/openwrt/out \
  $IMAGE
