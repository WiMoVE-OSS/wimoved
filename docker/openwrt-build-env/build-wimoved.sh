#!/bin/sh
set -ex
shopt -s globstar
#make "-j$(nproc)" V=sc prereq
./scripts/feeds update wimove_feed
./scripts/feeds install wimoved
make "-j$(nproc)" V=sc package/wimoved/compile
cp bin/targets/**/packages/wimoved*.ipk out
