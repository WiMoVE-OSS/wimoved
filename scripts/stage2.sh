#!/bin/sh
set -ex
make "-j$(nproc)" package/network/services/gaffa/compile
cp bin/targets/mvebu/cortexa9/packages/gaffa*.ipk out
