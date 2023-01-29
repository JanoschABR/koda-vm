#pragma once

#include "../shared.hpp"

/// A generic register
template <typename content_type>
class register_base {
    protected:
        content_type content;

        union data_union {
            public:
                content_type value;
                array<byte, sizeof(content_type)> data;

                explicit data_union (content_type x) {
                    value = x;
                }
                explicit data_union (array<byte, sizeof(content_type)> x) {
                    data = x;
                }
        };

    public:
        /// Set the data of this register
        void set (content_type value){
            content = value;
        }

        /// Get the data of this register
        content_type get (){
            return content;
        }

        /// Set the data of this register using raw bytes
        void set_raw (array<byte, sizeof(content_type)> data){
            set(data_union(data).value);
        }

        /// Get the data of this register as raw bytes
        array<byte, sizeof(content_type)> get_raw (){
            return data_union(get()).data;
        }

        /// Get the size of this register's data
        constexpr int size () {
            return sizeof(content_type);
        }

        /// Get a pointer to the content within this register
        content_type* get_pointer () {
            return &content;
        }
};

/// An 8-bit register (1 byte)
class x8register : public register_base<data8> {};

/// A 16-bit register (2 bytes)
class x16register : public register_base<data16> {};

/// A 32-bit register (4 bytes)
class x32register : public register_base<data32> {};

/// A collection of registers
template <int n, typename v_type>
class registers {
    typedef register_base<v_type> r_type;

    protected:
        r_type regs[n];

    public:
        registers () {
            // Initialize all the registers
            for (int i = 0; i < n; i++) {
                regs[i] = register_base<v_type>();
            }
        }

        /// Get the size of the data located at the data pointer
        int get_data_size () {
            return n * sizeof(r_type);
        }

        /// Get a pointer to the data
        byte* get_data_pointer () {
            return regs[0].get_pointer();
        }

        /// Returns true if the given address is valid
        bool is_valid_address (int address){
            return (address >= 0 && address < n);
        }

        /// Attempts to set the register at the address to the specified value. Returns false on failure.
        bool set (int address, v_type value){
            if (!is_valid_address(address)) return false;
            this->regs[address].set(value);
            return true;
        }

        /// Attempts to get the value of the register at the specified address. Returns false on failure.
        bool get (int address, v_type* pointer){
            if (!is_valid_address(address)) return false;
            *pointer = this->regs[address].get();
            return true;
        }

        /// Attempts to set the register at the address to the specified value using raw bytes. Returns false on failure.
        bool set_raw (int address, array<byte, sizeof(v_type)> data){
            if (!is_valid_address(address)) return false;
            this->regs[address].set_raw(data);
            return true;
        }

        /// Attempts to get the value of the register at the specified address as raw bytes. Returns false on failure.
        bool get_raw (int address, array<byte, sizeof(v_type)>* pointer){
            if (!is_valid_address(address)) return false;
            *pointer = this->regs[address].get_raw();
            return true;
        }

        /// Get a pointer to the register at the specified address. Returns nullptr if address is invalid.
        r_type* get_register (int address){
            if (!is_valid_address(address)) return nullptr;
            return this->regs[address];
        }

        /// Attempts to set the specified byte of the value belonging to the register at the specified address. Returns false on failure.
        // TODO: Implement set_raw8
        //bool set_raw8 (int address, data8 value, int index);

        /// Attempts to get the specified byte of the value belonging to the register at the specified address. Returns false on failure.
        // TODO: Implement get_raw8
        //bool get_raw8 (int address, int index, data8* pointer);
};

/// A collection of 8-bit registers (1 byte)
template <int n>
class x8registers : public registers<n, data8> {};

/// A collection of 16-bit registers (2 bytes)
template <int n>
class x16registers : public registers<n, data16> {};

/// A collection of 32-bit registers (4 bytes)
template <int n>
class x32registers : public registers<n, data32> {};