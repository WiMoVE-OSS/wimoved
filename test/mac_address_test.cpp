#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>

#include "MacAddress.h"

class MacAddressTest : public testing::TestWithParam<std::pair<std::string, uint64_t>> {};

TEST_P(MacAddressTest, SerializedIsOriginal) {
    auto [mac_address, number] = MacAddressTest::GetParam();
    ASSERT_EQ(MacAddress(mac_address).string(), mac_address);
}

TEST_P(MacAddressTest, NumberCorrect) {
    auto [mac_address, number] = MacAddressTest::GetParam();
    ASSERT_EQ(MacAddress(mac_address).number(), number);
}

using test_pair = std::pair<std::string, uint64_t>;

INSTANTIATE_TEST_SUITE_P(ExampleMacAddresses, MacAddressTest,
                         testing::Values(test_pair{"00:00:00:00:00:00", 0}, test_pair{"00:00:00:00:00:01", 1},
                                         test_pair{"00:00:00:00:00:0f", 15}, test_pair{"00:00:00:00:00:0F", 15},
                                         test_pair{"00:00:00:00:00:20", 32}, test_pair{"00:00:00:00:04:00", 1024},
                                         test_pair{"00:00:00:00:80:00", 32768},
                                         test_pair{"01:00:00:00:00:00", 1099511627776}));