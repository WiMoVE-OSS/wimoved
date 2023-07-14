ARG TAG=mvebu-cortexa9-22.03.3
FROM openwrt/sdk:$TAG

RUN ./scripts/feeds update -a
COPY --chown=buildbot:buildbot MakefilePrometheus /builder/my_packages/libs/prometheuscpp/Makefile
RUN chown -R buildbot:buildbot /builder/my_packages
# Create a custom feed
RUN echo "src-link my_packages /builder/my_packages" >> /builder/feeds.conf.default
RUN ./scripts/feeds update my_packages

RUN make defconfig

RUN ./scripts/feeds install prometheuscpp
RUN make package/feeds/my_packages/prometheuscpp/compile -j "$(nproc)"

RUN ./scripts/feeds install libnl
RUN make package/libnl/compile -j "$(nproc)"

COPY build-wimoved.sh /builder/
CMD bash /builder/build-wimoved.sh
