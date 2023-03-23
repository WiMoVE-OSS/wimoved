#include "BridgePerVxlanRenderer.h"

#include <iostream>

#include "logging/loginit.h"
#include "metrics/MetricsManager.h"

BridgePerVxlanRenderer::BridgePerVxlanRenderer()
    : station_gauge(MetricsManager::get_instance().get_station_gauge()),
      vni_gauge(MetricsManager::get_instance().get_vni_gauge()) {}

void BridgePerVxlanRenderer::setup_station(const Station& station) {
    std::lock_guard g(renderer_mutex);
    WMLOG(DEBUG) << "Calling: setup_station(" << station << ")";

    uint32_t vni = station.vni();
    socket.create_vxlan_iface(vni);
    socket.create_bridge_for_vni(vni);
    socket.add_iface_bridge("bridge" + std::to_string(vni), "vxlan" + std::to_string(vni));

    if (not station.vlan_id.has_value()) {
        throw std::runtime_error("The station " + station.mac.string() + " has no vlan_id");
    }

    socket.add_iface_bridge("bridge" + std::to_string(station.vni()), station.vlan_interface_name());
}

void BridgePerVxlanRenderer::cleanup(const std::function<std::vector<Station>()>& get_stations) {
    std::lock_guard g(renderer_mutex);
    std::unordered_set<uint32_t> connected_station_vnis(0);
    WMLOG(DEBUG) << "Starting cleanup.";
    int sta_counter = 0;
    for (auto& station : get_stations()) {
        connected_station_vnis.emplace(station.vni());
        sta_counter++;
    }
    vni_gauge.Set(double(connected_station_vnis.size()));
    station_gauge.Set(sta_counter);
    auto existing_interfaces = socket.interface_list();
    for (const auto& vni : connected_station_vnis) {
        existing_interfaces.erase(vni);
    }
    for (const auto vni : existing_interfaces) {
        WMLOG(DEBUG) << "Deleting VNI " << vni;
        try {
            socket.delete_interface("vxlan" + std::to_string(vni));
        } catch (const std::exception&) {
            WMLOG(ERROR) << "Could not delete vxlan interface vni: " << vni;
        }
        try {
            socket.delete_interface("bridge" + std::to_string(vni));
        } catch (const std::exception&) {
            WMLOG(ERROR) << "Could not delete bridge vni: " << vni;
        }
    }
    WMLOG(DEBUG) << "Cleanup finished, " << sta_counter << " stations connected.";
}
