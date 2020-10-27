#include "Util.h"

#include <gtest/gtest.h>
#include <arpa/inet.h>

TEST(UtilTest, NtoNllTest) {
    uint64_t value = 0xaabbccddeeff0011;
    util::htonT(&value);
    if (htonl(47) == 47) {
        ASSERT_EQ(0xaabbccddeeff0011, value);
    } else {
        ASSERT_EQ(0x1100ffeeddccbbaa, value);
    }

    uint64_t oldValue = value;
    util::htonT(&value);
    util::htonT(&value);
    ASSERT_EQ(oldValue, value);
}
