#LABEL org.opencontainers.image.source https://github.com/wimove-oss/wimoved
FROM ubuntu:lunar
RUN apt-get update && apt-get install -y --no-install-recommends cmake prometheus-cpp-dev libnl-3-dev libnl-route-3-dev clang clang-tidy clang-format make && apt-get clean autoclean && apt-get autoremove --yes && rm -rf /var/lib/apt/lists/* && rm -rf /var/lib/{cache,log}/
