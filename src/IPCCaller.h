#ifndef GAFFA_IPCCALLER_H
#define GAFFA_IPCCALLER_H


#include "IPCSocket.h"

class IPCCaller {
public:
    explicit IPCCaller(const std::string& iface);
    uint32_t vlan_for_station(const std::string& station_mac);
    std::vector<std::string> connected_stations();
private:
    IPCSocket socket;

};


#endif //GAFFA_IPCCALLER_H
