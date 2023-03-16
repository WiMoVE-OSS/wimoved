
# WiMoVE - Wireless Mobility through VXLAN EVPN

WiMoVE is a scalable Wi-Fi System that partitions all stations in separate overlay L2 domains, to limit the amount of wireless L2 broadcast traffic. In large Wi-Fi systems broadcast traffic can take up large amounts of airtime. A great talk, that explains the problem in more detail is available [here](https://www.youtube.com/watch?v=v8y-r9JBhmw). In comparison to other techniques like broadcast suppression our solution is transparent to the user and built with standardized technology. With our solution it will be possible to mix and match access points from different vendors that support OpenWRT. Handover is crucial in Wi-Fi systems to have a great user experience. For this reason the overlay L2 domain follows the station around and gets resized on demand. To build WiMoVE we use BGP EVPN with VXLAN as encapsulation. WiMove is a bachelor project at the chair [Internet Technologies and Softwarization](https://hpi.de/forschung/fachgebiete/internet-technologien-und-softwarization.html) at [HPI](https://hpi.de/).

## System architecture

In this section we explain the different components of our system on a high level with an exmaple. In this scenario we have four access points and four connected stations. Those four stations are partitioned in three overlay networks. Every station is connected to a different access point.

![image](https://user-images.githubusercontent.com/39831045/225569469-2fa88c14-5986-4b31-88a9-e9cfd6b296c7.png)

The different colors show the different overlay L2 domains the stations are assigned. An overlay L2 domain consists of a small number of stations and one gateway that provides L3 connectivity to other networks. To which overlay network a station is assigned is determined by its MAC address. As an advanced feature set we might add an optional central service, that is responsible for the assignment. Because an access point is only part of those overlay networks it has a station connected in, after a handover the overlay L2 domains might change and look like this:

![image](https://user-images.githubusercontent.com/39831045/225570985-02461e31-d007-4d1d-9026-94506ebe685c.png)

The system is fully transparent to the stations. It has normal unicast, multicast and broadcast L2 capabilities in the overlay networks. 

We use BGP EVPN as a control plane with a central route reflector.

![image](https://user-images.githubusercontent.com/39831045/225573570-30b6a90d-f49c-442e-b887-f41cffadea9e.png)

The black lines are iBGP peerings. The route reflector distributes received BGP updates to all other peers. It is possible to use BGP topologies without a central route reflector or with multiple route reflectors, but we haven't tested such topologies yet.

A more in depth explanation of our design decisions is available in our [design document](Link to desgin document). 

## State of this software

Currently we support OpenWRT with the following architectures: `ramips-mt7621`, `mvebu-cortexa9`. We test our software on the following Access Points: `ZyXEL NWA50AX` and `Linksys WRT1900ACS`. If you need support for another OpenWRT architecture feel free to open an issue. We'll consider adding another build pipeline. 
 
This software is still work in progress and subject to change. If you encounter an issue or have a feature request please open a ticket here on github.

## Setup

Our documentation is still pretty early stage with some rough edges. If something doesn't work the way you expect or you get stuck please let us know, so we can improve our guides and help you with your issue. A detailed setup guide can be found in the WiKi [here](https://github.com/WiMoVE-OSS/WiMoVE/wiki/Setup)

## Acknowledgements

The article series of Vincent Bernat is a great starting point to learn about BGP EVPN with VXLAN. It helped us getting started so please check it out [Link](https://vincent.bernat.ch/en/blog/2017-vxlan-bgp-evpn)
