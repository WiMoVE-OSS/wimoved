#include <iostream>
#include "BridgePerVxlanRenderer.h"

BridgePerVxlanRenderer::~BridgePerVxlanRenderer() {
    std::cout << "~BridgePerVxlanRenderer()" << std::endl;
}

void BridgePerVxlanRenderer::setup_vni(uint32_t vni) {
    std::cout << "setup_vni(" << vni << ")" << std::endl;
}

void BridgePerVxlanRenderer::setup_station(const Station& station) {
    std::cout << "setup_station(" << station.mac << ")" << std::endl;
}

void BridgePerVxlanRenderer::cleanup(const std::vector<uint32_t> &vnis) {
    std::cout << "cleanup(";
    for (auto& vni : vnis) {
        std::cout << vni << " ";
    }
    std::cout << ")" << std::endl;
}
