ARG TAG=mvebu-cortexa9-22.03.3
FROM openwrtorg/sdk:$TAG

RUN mkdir -p /home/build/openwrt/package/prometheus/src/
COPY MakefilePrometheus /home/build/openwrt/package/prometheus/Makefile
COPY build-wimoved.sh /home/build/

RUN ./scripts/feeds update && \
    make defconfig && \
    ./scripts/feeds install libnl && \
    make -j$(nproc) package/libnl/compile && \
    wget https://github.com/jupp0r/prometheus-cpp/releases/download/v1.1.0/prometheus-cpp-with-submodules.tar.gz && \
    tar -xzf prometheus-cpp-with-submodules.tar.gz -C /home/build/openwrt/package/prometheus/src/ --strip-components=1 && \
    make -j$(nproc) package/prometheus/compile && \
    rm -rf feeds

CMD bash /home/build/build-wimoved.sh


