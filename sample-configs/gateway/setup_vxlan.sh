#!/bin/bash
# Copied from Vincent Bernat's blog post:
for vni in {1..20}; do
    # Create VXLAN interface
    ip link add vxlan"${vni}" type vxlan
        id "${vni}" \
        dstport 4789 \
        nolearning
    # Create companion bridge
    brctl addbr br"${vni}"
    brctl addif br"${vni}" vxlan"${vni}"
    brctl stp br"${vni}" off
    ip link set up dev br"${vni}"
    ip a add 10.0."${vni}".1/24 dev br"${vni}"
    ip link set up dev vxlan"${vni}"
done