#include <iostream>
#include "BridgePerVxlanRenderer.h"

BridgePerVxlanRenderer::BridgePerVxlanRenderer() : socket() {

}

void BridgePerVxlanRenderer::setup_vni(uint32_t vni) {
    std::lock_guard g(renderer_mutex);
    std::cout << "setup_vni(" << vni << ")" << std::endl;
    socket.create_vxlan_iface(vni);
    socket.create_bridge_for_vni(vni);
    socket.add_iface_bridge("bridge" + std::to_string(vni), "vxlan" + std::to_string(vni));
}

void BridgePerVxlanRenderer::setup_station(const Station& station) {
    std::lock_guard g(renderer_mutex);
    if (not station.vlan_id.has_value()) {
        throw std::runtime_error("The station " + station.mac + " has no vlan_id");
    }
    std::cout << "setup_station(" << station.mac << ")" << std::endl;
    socket.add_iface_bridge("bridge" + std::to_string(station.vni()), station.vlan_interface_name());
}

void BridgePerVxlanRenderer::cleanup(const std::function<std::vector<Station>()> &get_stations) {
    std::lock_guard g(renderer_mutex);
    std::unordered_set<uint32_t > connected_station_vnis(0);
    std::cout << "cleanup(";
    for (auto& station : get_stations()) {
        std::cout << station.mac << " ";
        connected_station_vnis.emplace(station.vni());
    }
    std::cout << ")" << std::endl;
    auto existing_interfaces = socket.interface_list();
    for (const auto &vni : connected_station_vnis) {
        existing_interfaces.erase(vni);
    }
    for (const auto vni : existing_interfaces) {
       std::cout << "Delete " << vni << std::endl;
       try {
           socket.delete_interface("vxlan" + std::to_string(vni));
       } catch (const std::exception&) {
           std::cerr << "Could not delete vxlan interface vni: " << vni << std::endl;
       }
       try {
           socket.delete_interface("bridge" + std::to_string(vni));
       }
       catch (const std::exception&) {
           std::cerr << "Could not delete bridge vni: " << vni << std::endl;
       }
    }

}
