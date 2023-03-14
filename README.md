
# WiMoVE

WiMoVE is a scalable Wi-Fi System built with BGP EVPN. The design of the system is documented in our [design document](Link to desgin document). Currently we support OpenWRT with the following architectures: `ramips-mt7621`, `mvebu-cortexa9`. We test our software on the following Access Points: `ZyXEL NWA50AX` and `Linksys wrt1900ACS`. If you need support for another OpenWRT architecture feel free to open an issue. We'll consider adding another build pipeline.

## Setup

After following this guide you have a working instance of WiMoVE. You need at least one access point that can run OpenWRT. Additionally you need two hosts that can run [`frr`](https://github.com/FRRouting/frr). One of them is the route reflector and one is the gateway. The route reflector is part of the control plane and distributes the information between the access points and the gateways. The gateway is responsible to terminate the overlay networks and provide internet access. As an optional step you can also install dnsmasq as dhcp server on the gateway to provide ip addresses to the connected stations.

We use BGP EVPN as control plane and VXLAN as encapsulation technology. We use FRR an open source BGP implementation. A more in depth explanation of how BGP-EVPN works and how to configure it can be found in [this blog post](https://vincent.bernat.ch/en/blog/2017-vxlan-bgp-evpn) by Vincent Bernat. Most of the configuration for frr here is inspired by his blog post.

### Disclaimer

This software is still in development. Please write a bug report if you encounter any issues with our software. Currently only WPA-PSK with a single SSID and single interface is supported. Removing those limitations is on our roadmap :)

### Access Point Setup

#### Installing WiMoVE

1. Download the package for the matching architecture for your access point.
1. Verify that you have ssh connectivity to the access point. [OpenWRT guide for ssh](https://openwrt.org/docs/guide-quick-start/sshadministration)
1. Copy the package via SCP from your computer to the access point `scp -O <Path to WiMoVE on your machine> <access-point>`
1. Log-In via ssh on to your access point.
1. Install the package on your access point `opkg update && opkg install <Path to WiMoVE>`. An internet connection is required for this step in order to fetch dependencies.
1. Run `wimove`. You should see an error message, which shows that wimove is successfully installed.

#### FRR

This guide assumes the access point is running OpenWRT and the WiMoVE package has been installed.

1. Install `frr` via `opkg update && opkg install frr-bgpd frr-zebra frr-watchfrr frr-vtysh`
1. Copy `sample-configs/access-points/daemons` and `sample-configs/access-points/frr.conf` to the directory `/etc/frr/`.
1. Replace placeholder values in sample configs with actual values.
1. Restart `frr` by running `service frr restart`

#### Hostapd

The hostapd config will be generated for you from `/etc/config/wireless`. 

1. It is important that full version of hostapd is installed. The default package for hostapd does *not* work with our software. The correct package is named `hostapd`
1. Configure a Wi-Fi network with `WPA-PSK` security via the web interface.
1. The file `/etc/config/wireless` should now contain a `wifi-iface` section. The following options are required in this section. 

```text
    option isolate '1'
    option per_sta_vif '1'
    option vlan_file '/etc/hostapd.vlan'
```

1. Create the file `/etc/hostapd.vlan` with the following content:

```text
*   vlan#
```

1. Restart the access point

#### Configure WiMoVE

The default path for WiMoVE config is `/etc/wimove/config`. It can be configured as the first command line argument of WiMoVE. A default configuration file is available at `sample-configs/access-points/wimove`. There are listed all configuration options with the default values and an explanation. Right now there is almost no configuration. If you miss any configuration option, please feel free to open an issue.

After a successful configuration, you can run `wimove` and it should start without any errors. When connecting with a client, you should see log messages.

### Route Reflector setup

In addition to the access points we use a route reflector to build our control plane. It is possible to setup the system without a route reflector, if you are interested in such a setup just drop us a message. A route reflector can be any kind of machine where frr can be installed. It just needs a L3 connection to the access points.

#### FRR

1. Install `frr` on the route reflector.
1. Copy `sample-configs/route-reflector/daemons` and `sample-configs/route-reflector/frr.conf` to the directory `/etc/frr/`. Check the permissions and ownership afterwards with `ls -l /etc/frr`. They should look like this:

```bash
-rw-r-----   1 frr  frr daemons
-rw-r-----   1 frr  frr  frr.conf
```

1. Replace placeholder values in sample configs with actual values
1. Restart `frr`

You can verify the peering with the access point by running:

```bash
vtysh
show bgp neighbors
```

It should say that the connection with the access point is established.

### Gateway setup

The gateway is responsible for forwarding the internet traffic.

#### FRR

1. Install `frr` on the gateway.
1. Copy `sample-configs/gateway/daemons` and `sample-configs/gateway/frr.conf` to the directory `/etc/frr/`. Check the permissions and ownership afterwards with `ls -l /etc/frr`. They should look like this:

```bash
-rw-r-----   1 frr  frr daemons
-rw-r-----   1 frr  frr  frr.conf
```

1. Replace placeholder values in sample configs with actual values
1. Restart `frr`

#### VXLAN interfaces

1. Create all vxlan interfaces with the script `sample-configs/gateway/setup_vxlan.sh`. The script creates one bridge and one vxlan interface for every vni.

An alternative approach to using this script is using `netplan`. An advantage is that the interfaces will be created at boot. A sample netplan configuration can be found in `sample-configs/gateway/netplan.config`.

#### NFtables

1. Install `nftables`
1. Copy `sample-configs/gateway/nftables.conf`. Adapt the variables vxlans and uplinks according to your setup.

#### Dnsmasq

1. Install `dnsmasq` as dhcp-server on the gateway.
1. Copy dnsmasq config. `sample-configs/gateway/dnsmasq.conf`. This is a config file matching the default setup of `sample-configs/gateway/setup_vxlan.sh`. If you change anything within this script, the config probably has to be updated.
1. Restart dnsmasq.

## Development Setup WiMoVE

A possible workflow is to update the code of WiMoVE and use the build it with `scripts/build_openwrt.sh` and install the package on an access point. This can be pretty time consuming for many small changes. After following this guide, you can compile WiMoVE directly for your host system. Currently compiling hostapd for your host system is only possible on Linux.

### Setup hostapd

1. The build of hostapd has to support vlans. Check this if you encounter issues with your installation.
1. Use the sample config in `sample-configs/development/hostapd.conf`.
1. Replace the placeholder values in the config.
1. Create a file `/etc/hostapd/hostapd.vlan` with the content:

```text
*   vlan#
```

1. Restart hostapd.

### Build WiMoVE yourself

1. Clone the repo onto your computer
1. Install `libnl`, `libnl-route` [Repo](https://github.com/thom311/libnl) and `prometheus-cpp` [Repo](https://github.com/jupp0r/prometheus-cpp). The best installation method for those dependencies depends on your platform. For many platforms packages for `libnl` and `libnl-route` might be provided. `prometheus-cpp` has to be built according to the README.
1. Build the project by running `cmake .` followed by `make -j$(nproc)`.
1. You now have the WiMoVE binary for your architecture

This binary probably won't work on your access point because it has a different architecture.You have to use cross compiling to get a compatible binary.

### Formatting

Format the source files by running

```bash
find src -iname *.h -o -iname *.cpp | xargs clang-format -i
```
