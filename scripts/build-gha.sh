#!/bin/bash
mkdir -p out || true
chmod ogu+rw out
docker run --rm \
  -v "$(pwd)"/scripts/openwrtfiles:/home/build/openwrt/package/network/services/wimoved \
  -v "$(pwd)"/CMakeLists.txt:/home/build/openwrt/package/network/services/wimoved/src/CMakeLists.txt \
  -v "$(pwd)"/vendor:/home/build/openwrt/package/network/services/wimoved/src/vendor \
  -v "$(pwd)"/src:/home/build/openwrt/package/network/services/wimoved/src/src \
  -v "$(pwd)"/out:/home/build/openwrt/out \
  $IMAGE

