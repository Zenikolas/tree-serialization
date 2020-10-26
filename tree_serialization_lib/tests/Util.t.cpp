#include "Util.h"

#include <gtest/gtest.h>
#include <arpa/inet.h>

TEST(UtilTest, NtoNllTest) {
    uint64_t value = 0xaabbccddeeff0011;
    if (htonl(47) == 47) {
        ASSERT_EQ(value, util::htonll(value));
    } else {
        ASSERT_EQ(0x1100ffeeddccbbaa, util::htonll(value));
    }

    ASSERT_EQ(value, util::htonll(util::htonll(value)));
}
