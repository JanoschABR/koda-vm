#pragma once
#include "../shared.hpp"

template <int size> class x8memory {
    protected:
        array<byte, size> data = array<byte, size>();

    public:
        int get_data_size () {
            return size;
        }

        byte* get_data_pointer () {
            return &data[0];
        }

        bool is_valid_address (int address) {
            return (address >= 0 && address < size);
        }
        bool is_valid_address_range (int address, int length) {
            if (!is_valid_address(address)) return false;
            if (length <= 0) return false;
            return is_valid_address(address + (length - 1));
        }

        bool write (int address, byte _data) {
            if (!is_valid_address(address)) return false;
            data[address] = _data;
            return true;
        }

        bool write (int address, byte* _data, int length) {
            if (!is_valid_address_range(address, length)) return false;
            for (int i = 0; i < length; i++) {
                data[address + i] = _data[i];
            }
            return true;
        }

        template <int length>
        bool write (int address, array<byte, length> _data) {
            return write(address, &_data[0], length);
        }

        bool read (int address, byte* pointer) {
            if (!is_valid_address(address)) return false;
            *pointer = this->data[address];
            return true;
        }

        bool read (int address, byte* pointer, int length, int offset = 0) {
            if (!is_valid_address_range(address, length)) return false;
            for (int i = 0; i < length; i++) {
                pointer[offset + i] = data[address + i];
            }
            return true;
        }

        bool copy (int address, int length, int target_address) {
            if (!is_valid_address_range(address, length)) return false;
            if (!is_valid_address_range(target_address, length)) return false;

            for (int i = 0; i < length; i++) {
                data[target_address + i] = data[address + i];
            }
            return true;
        }
};