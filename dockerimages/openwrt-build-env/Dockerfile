ARG TAG=mvebu-cortexa9-22.03.3
FROM openwrtorg/sdk:$TAG

RUN ./scripts/feeds update && \
    make defconfig && \
    ./scripts/feeds install libnl && \
    make -j$(nproc) package/libnl/compile

RUN mkdir -p /home/build/openwrt/package/prometheus/src/ && \
    wget https://github.com/jupp0r/prometheus-cpp/releases/download/v1.1.0/prometheus-cpp-with-submodules.tar.gz && \
    tar -xzf prometheus-cpp-with-submodules.tar.gz -C /home/build/openwrt/package/prometheus/src/ --strip-components=1

COPY MakefilePrometheus /home/build/openwrt/package/prometheus/Makefile
COPY build_wimoved.sh /home/build/

RUN make -j$(nproc) package/prometheus/compile

CMD bash /home/build/build_wimoved.sh


