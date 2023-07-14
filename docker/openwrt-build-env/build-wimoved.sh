#!/bin/bash
set -ex
shopt -s globstar
./scripts/feeds update wimove_feed
./scripts/feeds install wimoved
make "-j$(nproc)" V=sc package/wimoved/compile
cp bin/targets/**/packages/wimoved*.ipk out
