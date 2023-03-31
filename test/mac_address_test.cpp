#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>

#include "MacAddress.h"

using validation_test_pair = std::pair<std::string, bool>;

class MacAddressValidationTest : public testing::TestWithParam<validation_test_pair> {};

TEST_P(MacAddressValidationTest, AddressValid) {
    auto [mac_address, valid] = MacAddressValidationTest::GetParam();

    if (valid) {
        ASSERT_NO_THROW(MacAddress(mac_address).string());
    } else {
        ASSERT_THROW(MacAddress(mac_address).string(), std::runtime_error);
    }
}

INSTANTIATE_TEST_SUITE_P(
    ExampleMacAddresses, MacAddressValidationTest,
    testing::Values(validation_test_pair{"00:00:00:00:00:00", true}, validation_test_pair{"01:23:45:67:89:ab", true},
                    validation_test_pair{"cd:ef:AB:CD:EF:00", true}, validation_test_pair{"00:00:00:00:00", false},
                    validation_test_pair{"00:00:00:00:00:00:00", false},
                    validation_test_pair{"0000:00:00:00:00", false}, validation_test_pair{"00:00:00:00:00:0", false},
                    validation_test_pair{"00:00:00:00:00:0g", false}));

using serialization_test_pair = std::pair<std::string, uint64_t>;

class MacAddressSerializationTest : public testing::TestWithParam<serialization_test_pair> {};

TEST_P(MacAddressSerializationTest, SerializedIsOriginal) {
    auto [mac_address, number] = MacAddressSerializationTest::GetParam();
    ASSERT_EQ(MacAddress(mac_address).string(), mac_address);
}

TEST_P(MacAddressSerializationTest, NumberCorrect) {
    auto [mac_address, number] = MacAddressSerializationTest::GetParam();
    ASSERT_EQ(MacAddress(mac_address).number(), number);
}

INSTANTIATE_TEST_SUITE_P(ExampleMacAddresses, MacAddressSerializationTest,
                         testing::Values(serialization_test_pair{"00:00:00:00:00:00", 0},
                                         serialization_test_pair{"00:00:00:00:00:01", 1},
                                         serialization_test_pair{"00:00:00:00:00:0f", 15},
                                         serialization_test_pair{"00:00:00:00:00:0F", 15},
                                         serialization_test_pair{"00:00:00:00:00:20", 32},
                                         serialization_test_pair{"00:00:00:00:04:00", 1024},
                                         serialization_test_pair{"00:00:00:00:80:00", 32768},
                                         serialization_test_pair{"01:00:00:00:00:00", 1099511627776}));