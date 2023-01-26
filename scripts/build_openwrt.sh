if [ "$OPENWRT_DIR" = "" ]; then
  echo 'OPENWRT_DIR is not set' 1>&2
  exit 1
fi

if [ ! -d "$OPENWRT_DIR" ]; then
  echo 'OPENWRT_DIR does not exist' 1>&2
  exit 1
fi

gaffa_dir="$(pwd)/$(dirname -- "$0")/.."

mkdir -p "$OPENWRT_DIR/package/network/services/gaffa"
rm -rf "$OPENWRT_DIR/package/network/services/gaffa/*"
cp "$gaffa_dir/scripts/Makefile" "$OPENWRT_DIR/package/network/services/gaffa"
cp -r "$gaffa_dir" "$OPENWRT_DIR/package/network/services/gaffa/src"

rm "$OPENWRT_DIR/package/network/services/gaffa/src/CMakeCache.txt"
rm -r "$OPENWRT_DIR/package/network/services/gaffa/src/cmake_install.cmake"
rm -r "$OPENWRT_DIR/package/network/services/gaffa/src/CMakeFiles"

(
cd "$OPENWRT_DIR" || exit 1
make "-j$(nproc)" package/network/services/gaffa/compile
)

(
scp -O "$OPENWRT_DIR/bin/targets/mvebu/cortexa9/packages/gaffa_1_arm_cortex-a9_vfpv3-d16.ipk" acs4:
ssh acs4 opkg install ./gaffa_1_arm_cortex-a9_vfpv3-d16.ipk
)