#include "Configuration.h"

#include <gtest/gtest.h>

class ConfigurationTest : public testing::Test {
   public:
    static Configuration& from_string(const std::string& string) {
        std::stringstream stream(string);
        Configuration& config = Configuration::get_instance();
        config.apply_config_file(ConfigParser(stream));
        return config;
    }
};

TEST(ConfigurationTest, EmptyConfiguration) {
    Configuration& config = ConfigurationTest::from_string("");
    ASSERT_EQ(config.max_vni, Configuration::DEFAULT_MAX_VNI);
    ASSERT_EQ(config.hapd_group, Configuration::DEFAULT_HAPD_GROUP);
    ASSERT_EQ(config.hapd_sockdir, Configuration::DEFAULT_HAPD_SOCKDIR);
    ASSERT_EQ(config.log_path, Configuration::DEFAULT_LOG_PATH);
    ASSERT_EQ(config.socknames, Configuration::DEFAULT_SOCKNAMES);
    ASSERT_EQ(config.cleanup_interval, Configuration::DEFAULT_CLEANUP_INTERVAL);
}

TEST(ConfigurationTest, MaxVNI) {
    Configuration& config = ConfigurationTest::from_string("max_vni=5");
    ASSERT_EQ(config.max_vni, 5);
}

TEST(ConfigurationTest, HapdGroup) {
    Configuration& config = ConfigurationTest::from_string("hapd_group=network");
    ASSERT_EQ(config.hapd_group, "network");
}

TEST(ConfigurationTest, HapdSockdir) {
    Configuration& config = ConfigurationTest::from_string("hapd_sockdir=/");
    ASSERT_EQ(config.hapd_sockdir, "/");
}

TEST(ConfigurationTest, LogPath) {
    Configuration& config = ConfigurationTest::from_string("log_path=/wimove.log");
    ASSERT_EQ(config.log_path, "/wimove.log");
}

TEST(ConfigurationTest, SockNames) {
    Configuration& config = ConfigurationTest::from_string("sockets=wlan0,wlan1,wlan2");
    std::vector<std::string> socknames{"wlan0", "wlan1", "wlan2"};
    ASSERT_EQ(config.socknames, socknames);
}

TEST(ConfigurationTest, CleanupInterval) {
    Configuration& config = ConfigurationTest::from_string("cleanup_interval=1");
    ASSERT_EQ(config.cleanup_interval, 1);
}

TEST(ConfigurationTest, NewlineSeparates) {
    Configuration& config = ConfigurationTest::from_string("max_vni=5\n\nhapd_group=network");
    ASSERT_EQ(config.max_vni, 5);
    ASSERT_EQ(config.hapd_group, "network");
}

TEST(ConfigurationTest, KeySpacingIgnored) {
    Configuration& config = ConfigurationTest::from_string(" hapd_group =network");
    ASSERT_EQ(config.hapd_group, "network");
}

TEST(ConfigurationTest, ValueSpacingIgnored) {
    Configuration& config = ConfigurationTest::from_string("hapd_group= network ");
    ASSERT_EQ(config.hapd_group, "network");
}