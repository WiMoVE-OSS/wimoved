//
// Created by aarons on 08.03.23.
//

#ifndef WIMOVED_CONFIGURATION_H
#define WIMOVED_CONFIGURATION_H

#include <cstdint>
#include <string>
#include <vector>

#include "ConfigParser.h"

class Configuration {
   public:
    static Configuration& get_instance() {
        static Configuration instance;
        return instance;
    }
    ~Configuration() = default;
    Configuration(Configuration const&) = delete;
    void operator=(Configuration const&) = delete;
    void populate(const ConfigParser& parser);

    // NOLINTBEGIN(readability-magic-numbers)
    uint32_t max_vni = 20;
    std::string hapd_sockdir = "/var/run/hostapd/";
    std::string hapd_group = "root";
    std::string log_path = "wimoved.log";
    std::vector<std::string> socknames = {"wlan0"};
    uint32_t cleanup_interval = 10;
    // NOLINTEND(readability-magic-numbers)
   private:
    Configuration() = default;
};

#endif  // WIMOVED_CONFIGURATION_H
