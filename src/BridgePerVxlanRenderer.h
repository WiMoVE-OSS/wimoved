#ifndef WM_BRIDGEPERVXLANRENDERER_H
#define WM_BRIDGEPERVXLANRENDERER_H

#include <prometheus/gauge.h>

#include <functional>
#include <mutex>

#include "NetworkRenderer.h"
#include "nl/Socket.h"

class BridgePerVxlanRenderer : public NetworkRenderer {
   private:
    nl::Socket socket;
    std::mutex renderer_mutex;
    prometheus::Gauge& station_gauge;
    prometheus::Gauge& vni_gauge;

   public:
    BridgePerVxlanRenderer();
    void setup_vni(uint32_t vni) override;
    void setup_station(const Station& station) override;
    void cleanup(const std::function<std::vector<Station>()>& get_stations) override;
};

#endif  // WM_BRIDGEPERVXLANRENDERER_H
