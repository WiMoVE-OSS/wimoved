ARG TAG=mvebu-cortexa9-22.03.3
FROM openwrt/sdk:$TAG

RUN ./scripts/feeds update -a
COPY --chown=buildbot:buildbot MakefilePrometheus /builder/wimove_feed/libs/prometheuscpp/Makefile
RUN chown -R buildbot:buildbot /builder/wimove_feed
# Create a custom feed
RUN echo "src-link wimove_feed /builder/wimove_feed" >> /builder/feeds.conf.default
RUN ./scripts/feeds update wimove_feed

RUN make defconfig

RUN ./scripts/feeds install prometheuscpp
RUN make package/feeds/wimove_feed/prometheuscpp/compile -j "$(nproc)"

RUN ./scripts/feeds install libnl
RUN make package/libnl/compile -j "$(nproc)"

COPY build-wimoved.sh /builder/
CMD bash /builder/build-wimoved.sh
