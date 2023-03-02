#ifndef GAFFA_BRIDGEPERVXLANRENDERER_H
#define GAFFA_BRIDGEPERVXLANRENDERER_H

#include <functional>
#include <mutex>

#include "NetworkRenderer.h"
#include "nl/Socket.h"

class BridgePerVxlanRenderer : public NetworkRenderer {
   private:
    nl::Socket socket;
    std::mutex renderer_mutex;

   public:
    BridgePerVxlanRenderer();
    void setup_vni(uint32_t vni) override;
    void setup_station(const Station& station) override;
    void cleanup(const std::function<std::vector<Station>()>& get_stations) override;
};

#endif  // GAFFA_BRIDGEPERVXLANRENDERER_H
