#!/bin/bash
mkdir -p out || true
chmod -R ogu+rw out
docker run --rm --ulimit nofile=1024:524288 \
  -e PKG_VERSION="$VERSION" \
  -v "$(pwd)"/openwrt/package:/builder/wimove_feed/network/services/wimoved \
  -v "$(pwd)"/CMakeLists.txt:/builder/wimove_feed/network/services/wimoved/src/CMakeLists.txt \
  -v "$(pwd)"/vendor:/builder/wimove_feed/network/services/wimoved/src/vendor \
  -v "$(pwd)"/src:/builder/wimove_feed/network/services/wimoved/src/src \
  -v "$(pwd)"/out:/out \
  "$IMAGE"
