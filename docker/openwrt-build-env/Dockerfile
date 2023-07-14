ARG TAG=mvebu-cortexa9-22.03.3
FROM openwrt/sdk:$TAG


COPY --chown=buildbot:buildbot MakefilePrometheus /builder/my_packages/libs/prometheuscpp/Makefile
# Creating the source directory and putting the files there does not work
# Probably we shouldn't do this anyway but rather specify the tar.gt directly in the Makefile as source
# and let openWrt handle it
RUN chown -R buildbot:buildbot /builder/my_packages
RUN mkdir -p /builder/my_packages/libs/prometheuscpp/src/ && wget https://github.com/jupp0r/prometheus-cpp/releases/download/v1.1.0/prometheus-cpp-with-submodules.tar.gz && \
    tar -xzf prometheus-cpp-with-submodules.tar.gz -C /builder/my_packages/libs/prometheuscpp/src/ --strip-components=1

# Create a custom feed
RUN echo "src-link my_packages /builder/my_packages" >> /builder/feeds.conf.default

RUN ./scripts/feeds update -a
RUN    ./scripts/feeds install libnl
# I think this does not work since the package is called prometheus-client-cpp
RUN ./scripts/feeds install prometheuscpp && \
    make defconfig && \
    make -j$(nproc) package/libnl/compile && \
    make -j$(nproc) package/prometheuscpp/compile && \
    rm -rf feeds

COPY build-wimoved.sh /builder/
CMD bash /builder/build-wimoved.sh
