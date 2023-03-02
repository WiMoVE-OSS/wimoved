#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include "ConfigParser.h"

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

ConfigParser::ConfigParser(const std::string& config_path) {
    // Set default values
    config_options["hapd_sock"] = "/var/run/hostapd/wlan0";
    config_options["max_vni"] = "20";

    std::string line;
    std::ifstream config_file (config_path);
    if (!config_file.is_open()) {
        std::cerr << "Unable to read config file " << config_path << ". Using default config" << std::endl;
        return;
    }

    // Read Config
    std::string delimiter = "=";
    while ( getline (config_file, line) )
    {
        auto split = line.find(delimiter);
        trim(line);
        std::string token = line.substr(0, split);
        trim(token);
        // Make token lowercase
        std::transform(token.begin(), token.end(), token.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        // Check if token is already defined
        if (config_options.find(token) == config_options.end()) {
            std::cerr << "Unknown config option " << token << ". Skipping!!" << std::endl;
            continue;
        }

        std::string value = line.substr(split + 1, std::string::npos);
        trim(value);
        config_options[token] = value;
    }
}

std::string ConfigParser::get_config_option(const std::string& option) {
    auto got = config_options.find(option);
    if (got == config_options.end()) {
        throw std::runtime_error(std::string("Could not get option: ") + option);
    } else {
        return got->second;
    }
}


