#ifndef GAFFA_BRIDGEPERVXLANRENDERER_H
#define GAFFA_BRIDGEPERVXLANRENDERER_H

#include "NetworkRenderer.h"
#include "nl/Socket.h"

class BridgePerVxlanRenderer : public NetworkRenderer {
private:
    nl::Socket socket;
public:
    BridgePerVxlanRenderer();
    ~BridgePerVxlanRenderer();
    void setup_vni(uint32_t vni) override;
    void setup_station(const Station& station) override;
    void cleanup(const std::vector<uint32_t>& vnis) override;
};


#endif //GAFFA_BRIDGEPERVXLANRENDERER_H