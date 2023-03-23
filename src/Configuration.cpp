#include "Configuration.h"

#include <stdexcept>


const uint32_t Configuration::DEFAULT_MAX_VNI = 20;
const std::string Configuration::DEFAULT_HAPD_SOCKDIR = "/var/run/hostapd/";
const std::string Configuration::DEFAULT_HAPD_GROUP = "root";
const std::string Configuration::DEFAULT_LOG_PATH = "wimoved.log";
const std::vector<std::string> Configuration::DEFAULT_SOCKNAMES = {"wlan0"};
const uint32_t Configuration::DEFAULT_CLEANUP_INTERVAL = 10;

static void set_string_if_valid(const ConfigParser& parser, std::string& config_target, const std::string& key) {
    try {
        config_target = parser.get_config_string(key);
    } catch (const std::out_of_range& err) {
    }
}

static void set_uint32_if_valid(const ConfigParser& parser, uint32_t& config_target, const std::string& key) {
    try {
        config_target = parser.get_config_uint32(key);
    } catch (const std::out_of_range& err) {
    }
}

static void set_string_vector_if_valid(const ConfigParser& parser, std::vector<std::string>& config_target,
                                       const std::string& key) {
    try {
        config_target = parser.get_config_string_vector(key);
    } catch (const std::out_of_range& err) {
    }
}

void Configuration::populate(const ConfigParser& parser) {
    set_string_if_valid(parser, this->hapd_sockdir, "hapd_sockdir");
    set_string_if_valid(parser, this->hapd_group, "hapd_group");
    set_string_if_valid(parser, this->log_path, "log_path");
    set_uint32_if_valid(parser, this->cleanup_interval, "cleanup_interval");
    set_uint32_if_valid(parser, this->max_vni, "max_vni");
    set_string_vector_if_valid(parser, this->socknames, "sockets");
}