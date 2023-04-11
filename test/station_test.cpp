#include "Station.h"

#include <gtest/gtest.h>

#include "Configuration.h"

class StationTest : public testing::TestWithParam<std::tuple<MacAddress, uint32_t, uint32_t>> { };

TEST_P(StationTest, VniIsWithinRange) {
    auto [mac, min_vni, max_vni] = StationTest::GetParam();
    if (min_vni > max_vni) {
        return;
    }
    Configuration &config = Configuration::get_instance();
    config.min_vni = min_vni;
    config.max_vni = max_vni;
    Station station("wlan0", mac);
    ASSERT_LE(min_vni, station.vni());
    ASSERT_GE(max_vni, station.vni());
}

INSTANTIATE_TEST_SUITE_P(ExampleStationData, StationTest, testing::Combine(
                                                              testing::Values(MacAddress("00:00:00:00:00:00"),
                                                                              MacAddress("00:00:00:00:00:01"),
                                                                              MacAddress("00:00:00:00:00:0F"),
                                                                              MacAddress("00:00:00:00:00:20"),
                                                                              MacAddress("00:00:00:00:04:00"),
                                                                              MacAddress("00:00:00:00:80:00"),
                                                                              MacAddress("01:00:00:00:00:00")),
                                                              testing::Values(0, 1, 2, 3, 4, 5),
                                                                testing::Values(0, 1, 2, 3, 4, 5)));

