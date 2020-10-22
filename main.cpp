#include <iostream>
#include <fstream>

unsigned long long swapBytes(uint64_t src) {
    enum {
        INIT_TYPE = 0,
        LE_TYPE,
        BE_TYPE
    };

    static int typ = INIT_TYPE;
    unsigned char c;
    union {
        unsigned long long ull;
        unsigned char c[8];
    } x;
    if (typ == INIT_TYPE) {
        x.ull = 0x01;
        typ = (x.c[7] == 0x01ULL) ? BE_TYPE : LE_TYPE;
    }
    if (typ == BE_TYPE)
        return src;
    x.ull = src;
    c = x.c[0]; x.c[0] = x.c[7]; x.c[7] = c;
    c = x.c[1]; x.c[1] = x.c[6]; x.c[6] = c;
    c = x.c[2]; x.c[2] = x.c[5]; x.c[5] = c;
    c = x.c[3]; x.c[3] = x.c[4]; x.c[4] = c;
    return x.ull;
}

void write(const std::string_view filename, double value)
{
    std::ofstream ofs(filename.data(), std::ofstream::binary | std::ofstream::out);

    ofs << swapBytes(reinterpret_cast<uint64_t&>(value));
}

double read(const std::string_view filename)
{
    std::ifstream ifs(filename.data(), std::ifstream::binary | std::ifstream::in);
    uint64_t ret;
    ifs >> ret;
    ret = swapBytes(ret);

    return reinterpret_cast<double&>(ret);
}



int main() {
    const char* filename = "double_serialization.dat";
    double value = 100.021923123123000999999;
    write(filename, value);
    double valueCheck = read(filename);
    std::cout << valueCheck << std::endl;


    return 0;
}
