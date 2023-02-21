ARG TAG=mvebu-cortexa9-22.03.3
FROM openwrtorg/sdk:$TAG

RUN ./scripts/feeds update && \
    make defconfig && \
    ./scripts/feeds install libnl && \
    make -j$(nproc) package/libnl/compile

