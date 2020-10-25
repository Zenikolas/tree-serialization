#include "Util.h"

#include <algorithm>

namespace util {
    unsigned long long htonll(uint64_t src) {
        enum {
            INIT_TYPE = 0,
            LE_TYPE,
            BE_TYPE
        };

        static int type = INIT_TYPE;
        union {
            unsigned long long ull;
            unsigned char c[8];
        } x;
        if (type == INIT_TYPE) {
            x.ull = 0x01;
            type = (x.c[7] == 0x01ULL) ? BE_TYPE : LE_TYPE;
        }
        if (type == BE_TYPE)
            return src;
        x.ull = src;
        std::swap(x.c[0], x.c[7]);
        std::swap(x.c[1], x.c[6]);
        std::swap(x.c[2], x.c[5]);
        std::swap(x.c[3], x.c[4]);
        return x.ull;
    }
}
