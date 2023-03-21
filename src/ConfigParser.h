#ifndef WIMOVED_CONFIGPARSER_H
#define WIMOVED_CONFIGPARSER_H

#include <cstdint>
#include <string>
#include <unordered_map>

class ConfigParser {
   public:
    explicit ConfigParser(const std::string& config_path);
    std::string get_config_string(const std::string& option) const;
    uint32_t get_config_uint32(const std::string& option) const;

   private:
    std::unordered_map<std::string, std::string> config_options;
};

#endif  // WIMOVED_CONFIGPARSER_H
