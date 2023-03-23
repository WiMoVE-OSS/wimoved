#include "ConfigParser.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Configuration.h"
#include "logging/loginit.h"

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

ConfigParser::ConfigParser(const std::string &config_path) {
    std::string line;
    std::ifstream config_file(config_path);
    if (!config_file.is_open()) {
        WMLOG(ERROR) << "Unable to read config file " << config_path << ". Using default config";
        return;
    }

    // Read Config
    std::string delimiter = "=";
    while (getline(config_file, line)) {
        auto split = line.find(delimiter);
        trim(line);
        std::string token = line.substr(0, split);
        trim(token);
        // Make token lowercase
        std::transform(token.begin(), token.end(), token.begin(), [](unsigned char c) { return std::tolower(c); });

        std::string value = line.substr(split + 1, std::string::npos);
        trim(value);
        config_options[token] = value;
    }
    Configuration::get_instance().populate(*this);
}

std::string ConfigParser::get_config_string(const std::string &option) const {
    auto got = config_options.find(option);
    if (got == config_options.end()) {
        throw std::out_of_range(std::string("Could not get option: ") + option);
    } else {
        return got->second;
    }
}

uint32_t ConfigParser::get_config_uint32(const std::string &option) const {
    std::string value = this->get_config_string(option);
    uint64_t result = std::stoull(value);
    if (result > std::numeric_limits<uint32_t>::max()) {
        throw std::range_error("Number is too large for uint32_t. Config option " + option + " with value " + value);
    }
    return result;
}

std::vector<std::string> ConfigParser::get_config_string_vector(const std::string &option) const {
    std::string value = get_config_string(option);
    std::vector<std::string> strings;

    std::stringstream stream(value);
    while (stream.good()) {
        std::string substring;
        getline(stream, substring, ',');
        trim(substring);
        strings.push_back(substring);
    }
    return strings;
}
