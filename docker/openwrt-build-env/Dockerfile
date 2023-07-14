ARG TAG=mvebu-cortexa9-22.03.3
FROM openwrt/sdk:$TAG

RUN ./scripts/feeds update -a && \
    make defconfig

COPY --chown=buildbot:buildbot MakefilePrometheus /builder/wimove_feed/libs/prometheuscpp/Makefile
RUN chown -R buildbot:buildbot /builder/wimove_feed && \
    echo "src-link wimove_feed /builder/wimove_feed" >> /builder/feeds.conf.default && \
    ./scripts/feeds update wimove_feed

RUN ./scripts/feeds install prometheuscpp && \
    make V=s package/feeds/wimove_feed/prometheuscpp/compile -j "$(nproc)"

RUN ./scripts/feeds install libnl && \
    make V=s package/libnl/compile -j "$(nproc)"

COPY build-wimoved.sh /builder/
CMD bash /builder/build-wimoved.sh
