# WiMoVE - Wireless Mobility through VXLAN EVPN

![CI Status](https://github.com/WiMoVE-OSS/wimoved/actions/workflows/build.yml/badge.svg?branch=main)
![Linter Status](https://github.com/WiMoVE-OSS/wimoved/actions/workflows/linter.yml/badge.svg?branch=main)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/WiMoVE-OSS/wimoved/blob/main/LICENSE)

WiMoVE is a scalable Wi-Fi System that partitions all stations in separate overlay L2 domains to limit the amount of wireless L2 broadcast traffic.
In large Wi-Fi systems, broadcast traffic can take up large amounts of airtime.
A great talk that explains the problem in more detail is available [here](https://www.youtube.com/watch?v=v8y-r9JBhmw).
In WiMoVE, overlay L2 domains "follow" the stations, being resized on demand, thus preserving handover.

WiMoVE is built with standard network protocols, on top of open&#8209;source technology:

- The overlay networks use [BGP EVPN](https://www.rfc-editor.org/rfc/rfc8365.html) with [VXLAN](https://www.rfc-editor.org/rfc/rfc7348) encapsulation.
- All BGP speakers run [FRRouting](https://frrouting.org/) to exchange EVPN routes.
- The access points run [OpenWRT](https://openwrt.org/) with our custom, open&#8209;source daemon called [wimoved](https://github.com/wimove-oss/wimoved).

This solution allows for using commodity access points running OpenWRT for large&#8209;scale Wi&#8209;Fi deployments, even from different vendors.

## This repository

This repository contains the WiMoVE Access Point daemon wimoved.
The daemon is responsible for handling hostapd events and creating VXLAN interfaces.
WiMoVE consists of multiple other parts.
If you want to learn more about the architecture or create a full setup, take a look at our [documentation](https://wimove-oss.github.io/docs/).

## Supported architectures

Currently, we support OpenWRT with the architectures `ramips-mt7621` and `mvebu-cortexa9`.
We test our software on the Access Point models `ZyXEL NWA50AX` and `Linksys WRT1900ACS`.
If you need support for other OpenWRT architectures, feel free to open an issue.

## Development setup

The development setup is an easy way to test wimoved.
Please be aware that after following this guide, you do **not** have a full WiMoVE installation.
You can connect to the Wi-Fi but won't be connected to other devices or the Internet. If you want a complete WiMoVE setup, follow the [setup guide](https://wimove-oss.github.io/docs/) instead.
Currently, we only support Linux as a development platform.

### Setup hostapd

First, we will set up [hostapd](https://w1.fi/).

- Install hostapd using your distribution's package manager. Alternatively, you can build it from source, see [here](https://w1.fi/). By default, hostapd comes with VLAN support. If you encounter issues, make sure that it was compiled with `CONFIG_NO_VLAN=n`.
- Put the following in your `/etc/hostapd.conf`, replacing the placeholder values:

```text
    interface=<interface>
    ssid=<ssid>
    ieee80211d=1
    country_code=<Your country code>
    hw_mode=g
    ieee80211n=1
    channel=6
    beacon_int=1000
    dtim_period=2
    max_num_sta=255
    rts_threshold=-1
    fragm_threshold=-1
    macaddr_acl=0
    auth_algs=1
    ignore_broadcast_ssid=0
    wmm_enabled=0
    eapol_key_index_workaround=0
    eap_server=0
    wpa=2
    wpa_key_mgmt=WPA-PSK
    rsn_pairwise=CCMP
    wpa_passphrase=<Secret key>
    per_sta_vif=1
    vlan_file=/etc/hostapd/hostapd.vlan
```

- Create a file `/etc/hostapd/hostapd.vlan` with the following content: 

```text
*   vlan#
```
- Hostapd has to be started with the option `-g /var/run/hostapd/global`. For the service to work properly, you might have to edit the service file for hostapd. Run `systemctl status hostapd` to locate this file.
- Start hostapd, either with `systemctl start hostapd` or on the command line. You might need to stop `NetworkManager` before starting hostapd since the programs interfere with each other.

### Build WiMoVE

- Install [`libnl`](https://github.com/thom311/libnl). On a recent Linux system, the corresponding package is probably already installed.
- Install [`prometheus-cpp`](https://github.com/jupp0r/prometheus-cpp).
    - On Ubuntu, install the package `prometheus-cpp-dev`.
    - On Arch Linux, the package is available in the AUR as `prometheus-cpp-git`.
- Clone the repository by running `git clone https://github.com/WiMoVE-OSS/wimoved`.
- Build the project by running `cmake .` followed by `make -j$(nproc)`.
- Start wimoved by running `./wimoved`.

### Coding guidelines

Format the source files by running `make format`.
Lint the source files by running `make lint`.
Build the tests by running `make test`.
Run both checks and the tests by doing `make precommit`.
The coding guidelines are enforced via the CI pipeline.

As linting takes a long time, we recommend integrating `clang-tidy` into your editor.

## Sanitization

Build with sanitizers enabled by running `cmake . -DWIMOVED_SANITIZE=ON`.
For full stacktrace support, set the environment variable `LSAN_OPTIONS=fast_unwind_on_malloc=0:malloc_context_size=30`.