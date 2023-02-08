#include <iostream>
#include "BridgePerVxlanRenderer.h"

BridgePerVxlanRenderer::BridgePerVxlanRenderer() : socket() {

}

BridgePerVxlanRenderer::~BridgePerVxlanRenderer() {
    std::cout << "~BridgePerVxlanRenderer()" << std::endl;
}

void BridgePerVxlanRenderer::setup_vni(uint32_t vni) {
    std::cout << "setup_vni(" << vni << ")" << std::endl;
    socket.create_vxlan_iface(vni);
}

void BridgePerVxlanRenderer::setup_station(const Station& station) {
    std::cout << "setup_station(" << station.mac << ")" << std::endl;
}

void BridgePerVxlanRenderer::cleanup(const std::function<std::vector<Station>()> &get_stations) {
    std::cout << "cleanup(";
    for (auto& station : get_stations()) {
        std::cout << station.mac << " ";
    }
    std::cout << ")" << std::endl;

}
