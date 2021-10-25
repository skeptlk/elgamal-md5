#include "core/md5.h"

/**
 * Rotate n by d bits
 */
uint32 leftRotate(uint32 n, uint32 d)
{
    return (n << d) | (n >> (sizeof(n) * 8 - d));
}

template <typename T>
T swap_endian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

// s specifies the per-round shift amounts
const uint32 s[64] = {
    7, 12, 17, 22,   7, 12, 17, 22,   7, 12, 17, 22,   7, 12, 17, 22,
    5,  9, 14, 20,   5,  9, 14, 20,   5,  9, 14, 20,   5,  9, 14, 20,
    4, 11, 16, 23,   4, 11, 16, 23,   4, 11, 16, 23,   4, 11, 16, 23,
    6, 10, 15, 21,   6, 10, 15, 21,   6, 10, 15, 21,   6, 10, 15, 21,
};

uint32* md5(std::istream* input)
{
    uint32 K[64];

    // Use binary integer part of the sines
    // of integers (Radians) as constants:
    for (int i = 0; i < 64; ++i) {
        K[i] = floor(pow(2, 32) * abs(sin(i + 1)));
    }

    // Initialize variables:
    uint32 a0 = 0x67452301,   // A
           b0 = 0xefcdab89,   // B
           c0 = 0x98badcfe,   // C
           d0 = 0x10325476;   // D


    uint64 len = 0; // initial length of message in bytes
    char chunk[64];
    bool end = false;
    bool processLast = false; // should process last appended chunk

    while (!end || processLast)
    {
        input->clear();
        input->read((char *)chunk, 64);
        len += input->gcount();

        if (input->fail()) {
            end = true;
            int c = input->gcount();
            chunk[c] = 0x80;
            c++;

            if (c > 56) {
                while (c <= 64) {
                    chunk[c++] = 0;
                }
                processLast = true;
            } else {
                if (processLast) {
                    c = 0;
                    processLast = false;
                }
                while (c <= 56)
                    chunk[c++] = 0;
                // append original message length (in bits!)
                len *= 8;
                memcpy(chunk + 56, &len, 8);
            }
        }

        uint32 M[16];
        memcpy(M, chunk, sizeof(chunk));
        // initialize hash value for this chunk:
        uint32 A = a0,
               B = b0,
               C = c0,
               D = d0;

        // Main loop:
        for (int i = 0; i < 64; ++i)
        {
            uint32 F = 0, g = 0;
            if (0 <= i && i <= 15) {
                F = (B & C) | (~B & D);
                g = i;
            } else if (16 <= i && i <= 31) {
                F = (D & B) | (~D & C);
                g = (5 * i + 1) % 16;
            } else if (32 <= i && i <= 47) {
                F = B ^ C ^ D;
                g = (3 * i + 5) % 16;
            } else if (48 <= i && i <= 63) {
                F = C ^ (B | ~D);
                g = (7 * i) % 16;
            }
            F = F + A + K[i] + M[g];
            A = D;
            D = C;
            C = B;
            B = B + leftRotate(F, s[i]);
        }

        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;
    }

    uint32* hash = new uint32[4]{
        swap_endian<uint32>(a0),
        swap_endian<uint32>(b0),
        swap_endian<uint32>(c0),
        swap_endian<uint32>(d0)
    };
    return hash;
}

