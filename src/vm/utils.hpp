#pragma once
#include "shared.hpp"

namespace bin {
    // TODO: Optimize if necessary

    /*namespace unions {
        union bytes_to_short {
            byte data[2];
            short value;
        };
        union short_to_bytes {
            short value;
            array<byte, 2> data;
        };

        union bytes_to_int {
            byte data[4];
            int value;
        };
        union int_to_bytes {
            int value;
            array<byte, 2> data;
        };
    }

    short bytes_to_short (byte a, byte b) {
        return unions::bytes_to_short{b, a}.value;
    }
    short bytes_to_short (byte* start, int offset = 0) {
        return bytes_to_short(start[offset], start[offset + 1]);
    }
    array<byte, 2> short_to_bytes (short value) {
        return unions::short_to_bytes{value}.data;
    }

    int bytes_to_int (byte a, byte b, byte c, byte d) {
        return unions::bytes_to_int{d, c, b, a}.value;
    }
    int bytes_to_int (byte* start, int offset = 0) {
        return bytes_to_int(start[offset], start[offset + 1], start[offset + 2], start[offset + 3]);
    }
    array<byte, 2> int_to_bytes (int value) {
        return unions::int_to_bytes{value}.data;
    }*/


    short bytes_to_short (byte a, byte b) {
        short buffer = a;
        buffer <<= 8;
        buffer |= b;
        return buffer;
    }
    short bytes_to_short (byte* start, int offset = 0) {
        return bytes_to_short(start[offset], start[offset + 1]);
    }

    array<byte, 2> short_to_bytes (short value) {
        array<byte, 2> buffer = array<byte, 2>();
        buffer[0] = ((value & 0xFF00) >> 8);
        buffer[1] = (value & 0x00FF);
        return buffer;
    }



    int bytes_to_int (byte a, byte b, byte c, byte d) {
        int buffer = a;
        buffer = ((buffer << 8) | b);
        buffer = ((buffer << 8) | c);
        buffer = ((buffer << 8) | d);
        return buffer;
    }
    int bytes_to_int (byte* start, int offset = 0) {
        return bytes_to_int(start[offset], start[offset + 1], start[offset + 2], start[offset + 3]);
    }

    array<byte, 4> int_to_bytes (int value) {
        array<byte, 4> buffer = array<byte, 4>();
        buffer[0] = ((value & 0xFF00) >> (8 * 3));
        buffer[1] = ((value & 0xFF00) >> (8 * 2));
        buffer[2] = ((value & 0xFF00) >> (8 * 1));
        buffer[3] = ((value & 0xFF00) >> (8 * 0));
        return buffer;
    }



    void print_hex (byte* pointer, uint length) {
        for(int i=0; i < length; ++i)
            std::cout << full_length(byte) << std::hex << (int)pointer[i];
    }
}

template <size_t N, class T>
array<T,N> make_array(const T &v) {
    array<T,N> ret;
    ret.fill(v);
    return ret;
}