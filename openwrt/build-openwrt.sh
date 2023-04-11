#!/bin/bash
mkdir -p out || true
chmod -R ogu+rw out
docker run --rm \
  -e PKG_VERSION="$(VERSION)+beta$(date '+%Y-%m-%d-%H.%M.%S')" \
  -v "$(pwd)"/openwrt/package:/home/build/openwrt/package/network/services/wimoved \
  -v "$(pwd)"/CMakeLists.txt:/home/build/openwrt/package/network/services/wimoved/src/CMakeLists.txt \
  -v "$(pwd)"/vendor:/home/build/openwrt/package/network/services/wimoved/src/vendor \
  -v "$(pwd)"/src:/home/build/openwrt/package/network/services/wimoved/src/src \
  -v "$(pwd)"/out:/home/build/openwrt/out \
  "$($IMAGE)"
