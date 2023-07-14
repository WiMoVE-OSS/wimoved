#!/bin/bash
mkdir -p out || true
chmod -R ogu+rw out
docker run --rm --ulimit nofile=1024:524288 \
  -e PKG_VERSION="$VERSION" \
  -v "$(pwd)"/openwrt/package:/builder/my_packages/network/services/wimoved \
  -v "$(pwd)"/CMakeLists.txt:/builder/my_packages/network/services/wimoved/src/CMakeLists.txt \
  -v "$(pwd)"/vendor:/builder/my_packages/network/services/wimoved/src/vendor \
  -v "$(pwd)"/src:/builder/my_packages/network/services/wimoved/src/src \
  -v "$(pwd)"/out:/out \
  "$IMAGE"
