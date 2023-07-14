#!/bin/bash
mkdir -p out || true
chmod -R ogu+rw out
docker run --rm \
  -e PKG_VERSION="$VERSION" \
  -v "$(pwd)"/openwrt/package:/builder/package/network/services/wimoved \
  -v "$(pwd)"/CMakeLists.txt:/builder/package/network/services/wimoved/src/CMakeLists.txt \
  -v "$(pwd)"/vendor:/builder/package/network/services/wimoved/src/vendor \
  -v "$(pwd)"/src:/builder/package/network/services/wimoved/src/src \
  -v "$(pwd)"/out:/out \
  "$IMAGE"
