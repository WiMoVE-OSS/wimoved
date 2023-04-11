#include "Configuration.h"

#include <stdexcept>

#include "filesystem"
#include "logging/loginit.h"

const uint32_t Configuration::DEFAULT_MIN_VNI = 1;
const uint32_t Configuration::DEFAULT_MAX_VNI = 20;
const std::string Configuration::DEFAULT_HAPD_SOCKDIR = "/var/run/hostapd/";
const std::string Configuration::DEFAULT_HAPD_GROUP = "root";
const std::string Configuration::DEFAULT_LOG_PATH = "wimoved.log";
const std::vector<std::string> Configuration::DEFAULT_SOCKNAMES = {};
const uint32_t Configuration::DEFAULT_CLEANUP_INTERVAL = 10;

const std::string HAPD_GLOBAL_SOCK_NAME = "global";

Configuration Configuration::INSTANCE;

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

void Configuration::set_all_available_sockets_if_empty() {
    if (socknames.empty()) {
        for (const auto& entry : std::filesystem::directory_iterator(hapd_sockdir)) {
            if (entry.is_socket() && entry.path().filename() != HAPD_GLOBAL_SOCK_NAME) {
                socknames.emplace_back(entry.path().filename());
            }
        }
        if (socknames.empty()) {
            throw std::runtime_error("No sockets were configured and no sockets could be found.");
        }
        WMLOG(DEBUG) << "No sockets were configured. Using all " << socknames.size() << " sockets available.";
    }
}

void Configuration::apply_config_file(const ConfigParser& parser) {
    set_string_if_valid(parser, this->hapd_sockdir, "hapd_sockdir");
    set_string_if_valid(parser, this->hapd_group, "hapd_group");
    set_string_if_valid(parser, this->log_path, "log_path");
    set_uint32_if_valid(parser, this->cleanup_interval, "cleanup_interval");
    set_uint32_if_valid(parser, this->min_vni, "min_vni");
    set_uint32_if_valid(parser, this->max_vni, "max_vni");
    set_string_vector_if_valid(parser, this->socknames, "sockets");
}

void Configuration::apply_environment() {
    set_all_available_sockets_if_empty();
}

void Configuration::check_validity() const {
    if (min_vni > max_vni) {
        throw std::runtime_error("min_vni must be less than or equal to max_vni " + std::to_string(min_vni) + " > " +
                                 std::to_string(max_vni));
    }
}
