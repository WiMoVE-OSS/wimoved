#ifndef WIMOVED_CONFIGPARSER_H
#define WIMOVED_CONFIGPARSER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class Configuration;

class ConfigParser {
   public:
    static ConfigParser from_file(const std::string& config_path);

    ConfigParser() = default;
    explicit ConfigParser(std::istream& istream);
    std::string get_config_string(const std::string& option) const;
    uint32_t get_config_uint32(const std::string& option) const;
    std::vector<std::string> get_config_string_vector(const std::string& option) const;
    void populate_configuration(Configuration& config) const;

   private:
    std::unordered_map<std::string, std::string> config_options;
};

#include "Configuration.h"

#endif  // WIMOVED_CONFIGPARSER_H
