#ifndef GAFFA_CONFIGPARSER_H
#define GAFFA_CONFIGPARSER_H

#include <unordered_map>

class ConfigParser {
public:
    explicit ConfigParser(const std::string& config_path);
    std::string get_config_option(const std::string& option);
private:
    std::unordered_map<std::string, std::string> config_options;
};


#endif //GAFFA_CONFIGPARSER_H
