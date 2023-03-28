# WiMoVE - Wireless Mobility through VXLAN EVPN

[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/getcursor/cursor/blob/main/LICENSE)
![CI Status](https://github.com/WiMoVE-OSS/wimoved/actions/workflows/build.yml/badge.svg?branch=main)
![Linter Status](https://github.com/WiMoVE-OSS/wimoved/actions/workflows/linter.yml/badge.svg?branch=main)

WiMoVE is a scalable Wi-Fi System that partitions all stations in separate overlay L2 domains, to limit the amount of wireless L2 broadcast traffic. In large Wi-Fi systems broadcast traffic can take up large amounts of airtime. A great talk, that explains the problem in more detail is available [here](https://www.youtube.com/watch?v=v8y-r9JBhmw). The overlay L2 domains "follow" the stations, being resized on demand, thus preserving handover.

WiMoVE is built with standard network protocols, on top of open&#8209;source technology:

- The overlay networks use [BGP EVPN](https://www.rfc-editor.org/rfc/rfc8365.html) with [VXLAN](https://www.rfc-editor.org/rfc/rfc7348) encapsulation.
- All BGP speakers run [FRRouting](https://frrouting.org/) to exchange EVPN routes.
- The access points run [OpenWRT](https://openwrt.org/) with our custom, open&#8209;source daemon called [wimoved](https://github.com/wimove-oss/wimoved).

This solution allows for using commodity access points running OpenWRT for large&#8209;scale Wi&#8209;Fi deployments, even from different vendors.

This repository contains the open&#8209;source daemon wimoved of the WiMoVE system. If you want to learn more about the architecture of the system, take a look at our [docs](https://wimove-oss.github.io/docs/)

## Setup

Our documentation for the setup is still pretty early stage with some rough edges. If something doesn't work the way you expect or you get stuck please let us know, so we can improve our guides and help you with your issue. A hosted version of the setup guide is available [here](https://wimove-oss.github.io/docs/setup)

##  State of this software

:warning: This software is still work in progress and subject to change. :warning:

Currently we support OpenWRT with the following architectures: `ramips-mt7621`, `mvebu-cortexa9`. We test our software on the following Access Points: `ZyXEL NWA50AX` and `Linksys WRT1900ACS`. If you need support for another OpenWRT architecture feel free to open an issue. We'll consider adding another build pipeline. If you encounter an issue or have a feature request please open a ticket here on github. 

## Acknowledgements

The article series of Vincent Bernat is a great starting point to learn about BGP EVPN with VXLAN. It helped us get started so please check it out [here](https://vincent.bernat.ch/en/blog/2017-vxlan-bgp-evpn).
