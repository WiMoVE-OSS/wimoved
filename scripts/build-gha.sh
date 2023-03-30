#!/bin/bash
set -xe
chmod -R ogu+rw out

docker run --rm \
  -v "$(pwd)"scripts/openwrtfiles:/home/build/openwrt/package/network/services/wimoved \
  -v "$(pwd)"CMakeLists.txt:/home/build/openwrt/package/network/services/wimoved/src \
  -v "$(pwd)"vendor:/home/build/openwrt/package/network/services/wimoved/src \
  -v "$(pwd)"src:/home/build/openwrt/package/network/services/wimoved/src \
  -v "$(pwd)"/out:/home/build/openwrt/out \
  $IMAGE
