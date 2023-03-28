#ifndef WIMOVED_CONFIGURATION_H
#define WIMOVED_CONFIGURATION_H

#include <cstdint>
#include <string>
#include <vector>

class ConfigParser;

class Configuration {
   public:
    static Configuration& get_instance() {
        static Configuration instance;
        return instance;
    }
    ~Configuration() = default;
    Configuration(Configuration const&) = delete;
    void operator=(Configuration const&) = delete;
    void apply_config_file(const ConfigParser& parser);
    void apply_environment();

    static const uint32_t DEFAULT_MAX_VNI;
    static const std::string DEFAULT_HAPD_SOCKDIR;
    static const std::string DEFAULT_HAPD_GROUP;
    static const std::string DEFAULT_LOG_PATH;
    static const std::vector<std::string> DEFAULT_SOCKNAMES;
    static const uint32_t DEFAULT_CLEANUP_INTERVAL;

    uint32_t max_vni = DEFAULT_MAX_VNI;
    std::string hapd_sockdir = DEFAULT_HAPD_SOCKDIR;
    std::string hapd_group = DEFAULT_HAPD_GROUP;
    std::string log_path = DEFAULT_LOG_PATH;
    std::vector<std::string> socknames = DEFAULT_SOCKNAMES;
    uint32_t cleanup_interval = DEFAULT_CLEANUP_INTERVAL;

   private:
    Configuration() = default;
    void set_all_available_sockets_if_empty();
};

#include "ConfigParser.h"

#endif  // WIMOVED_CONFIGURATION_H
