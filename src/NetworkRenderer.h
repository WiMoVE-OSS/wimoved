#ifndef WIMOVED_NETWORKRENDERER_H
#define WIMOVED_NETWORKRENDERER_H

#include <cstdint>
#include <functional>
#include <vector>

#include "Station.h"

class NetworkRenderer {
   public:
    virtual ~NetworkRenderer() = default;
    virtual void setup_station(const Station& station) = 0;
    virtual void cleanup(const std::function<std::vector<Station>()>& get_stations) = 0;
};

#endif  // WIMOVED_NETWORKRENDERER_H
