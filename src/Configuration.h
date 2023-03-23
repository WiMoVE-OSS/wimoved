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
    Configuration(Configuration const&) = delete;
    void populate(const ConfigParser& parser);
    void operator=(Configuration const&) = delete;
    uint32_t max_vni = 20;
    std::string hapd_sockdir = "/var/run/hostapd/";
    std::string hapd_group = "root";
    std::string log_path = "wimoved.log";
    std::vector<std::string> socknames = {"wlan0"};
    uint32_t cleanup_interval = 10;

   private:
    Configuration() {}
    void set_string_if_valid(const ConfigParser& parser, std::string& config_target, const std::string& key);
    void set_uint32_if_valid(const ConfigParser& parser, uint32_t& config_target, const std::string& key);
    void set_string_vector_if_valid(const ConfigParser& parser, std::vector<std::string>& config_target,
                                    const std::string& key);
};

#endif  // WIMOVED_CONFIGURATION_H
