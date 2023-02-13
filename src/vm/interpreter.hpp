#pragma once

#include <stack>
#include <iostream>
#include <bitset>
#include "instructions/shared.hpp"
#include "instructions/map/instruction_map.hpp"
#include "memory/registers.hpp"
#include "utils.hpp"
#include "shared.hpp"
#include "memory/memory.hpp"
#include "specs.hpp"

class instruction_interpreter;
#define instruction_t_params array<byte, INSTR_DATA_SIZE> data, instruction_interpreter* owner
typedef void (*instruction_t)(instruction_t_params);


/// If set the interpreter/machine will stop executing
#define FLAG_HALT   (1 << 0)

/// Indicates that the shutdown was due to an error of some sort
#define FLAG_PANIC  (1 << 1)

/// If set the current address will not be incremented next clock cycle.
/// It should be used when modifying the current address and is automatically set when using the jump instructions.
#define FLAG_JUMP   (1 << 2)

/// Indicates that the last performed ALU operation resulted in a value that is 16-bit or greater
#define FLAG_ALU_16 (1 << 3)

/// The value that is put into the compare register if the values equal each other
#define COMPARE_EQUAL 0

/// The value that is put into the compare register if the first value is greater than the other
#define COMPARE_MORE 1

/// The value that is put into the compare register if the first value is less than the other
#define COMPARE_LESS 2



class instruction_interpreter {
    protected:
        string prefix;

        instruction_map* map = nullptr;
        x8registers<16> regs;
        x8memory<MEMORY_SIZE>* memory;

        byte flags = 0b00000000;
        x8register state_register = x8register();
        x8register compare_register = x8register();

        stack<int> address_stack = stack<int>();
        int current_address = 0;

    public:
        instruction_interpreter () {
            this->regs = x8registers<16>();
            this->memory = nullptr;
        }

        explicit instruction_interpreter (x8memory<MEMORY_SIZE>* memory_ptr) : instruction_interpreter() {
            this->memory = memory_ptr;
        }

        explicit instruction_interpreter (x8memory<MEMORY_SIZE>* memory_ptr, instruction_map* map) : instruction_interpreter(memory_ptr) {
            this->set_instruction_map(map);
        }

        /// Set the instruction_map used for interpreting the alu
        inline void set_instruction_map (instruction_map* _map) {
            this->map = _map;
        }

        /// Set the prefix that this interpreter uses when writing to std::cout
        inline void set_prefix (const string& _prefix) {
            prefix = _prefix;
        }

        /// Get a pointer to the memory used by this interpreter
        inline x8memory<MEMORY_SIZE>* get_memory () {
            return memory;
        }

        /// Get a pointer to the state_register register
        inline x8register* get_state_register () {
            return &state_register;
        }

        /// Get a pointer to the state_register register
        inline x8register* get_compare_register () {
            return &compare_register;
        }

        /// Jump to a certain address. This does not automatically set the JUMP flag.
        void jump(int address) {
            current_address = address;
        }

        /// Pushes the current address onto the stack, then jumps to the new address.
        void jump_onto_stack(int address) {
            address_stack.push(current_address);
            jump(address);
        }

        /// Jumps to the top most address on the stack, then pops it.
        void jump_from_stack() {
            jump(address_stack.top());
            address_stack.pop();
        }

        /// Interpret an instruction
        void interpret(array<byte, INSTR_FULL_SIZE> instruction_data) {

            // Don't do anything if the halt flag is set
            if (get_flag(FLAG_HALT)) return;

            std::cout << prefix << "instr     0x"; // Print the data
            bin::print_hex(&instruction_data[0], INSTR_FULL_SIZE);
            std::cout << std::endl;

            // Retrieve code from instruction_data
            int code = bin::bytes_to_short(&instruction_data[0]);

            // Make sure the code is valid
            if (this->map->is_valid_address(code)) {

                instruction_t instruction; // Get the instruction
                if(this->map->lookup(code, &instruction)) {

                    if (instruction == nullptr) {
                        panic(4);
                    }

                    // Retrieve parameter params from instruction_data
                    array<byte, INSTR_DATA_SIZE> params = array<byte, INSTR_DATA_SIZE>();
                    std::copy(instruction_data.begin() + 2, instruction_data.end(), &params[0]);

                    instruction(params, this);
                } else panic(2);
            } else panic(1);

            std::cout << prefix << "regs      0x"; // Print the data
            bin::print_hex((byte*)regs.get_data_pointer(), regs.get_data_size());
            std::cout << std::endl;

            // Check if we halted
            if (get_flag(FLAG_HALT)) {
                std::bitset<8> flags_bitset(flags);
                std::cout << prefix <<  "halt     "
                          << " address=0x" << full_length(int) << std::hex << current_address
                          << " flags=" << flags_bitset
                          << " state_register=0x" << full_length(char) << std::hex << (int)state_register.get()
                          << "\n" << std::endl;
                return;
            }

            // Only report done if we haven't panicked
            if (!get_flag(FLAG_PANIC)) {

                std::bitset<8> flags_bitset(flags);
                std::cout << prefix <<  "ok       "
                          << " address=0x" << full_length(int) << std::hex << current_address
                          << " flags=" << flags_bitset
                          << " instruction=0x" << full_length(short) << std::hex << code
                          << " state_register=0x" << full_length(char) << std::hex << (int)state_register.get()
                          << "\n" << std::endl;
            }
        }

        /// Clear all flags
        void clear_flags() {
            this->flags = 0;
        }

        /// Enable or disable a flag
        void set_flag(int flag, bool value = true) {
            this->flags = ((this->flags & ~flag) | ((value ? 0xFFFF : 0x0000) & flag));
        }

        /// Get the state_register of a flag
        bool get_flag(int flag) {
            return (this->flags & flag);
        }

        /// Set the value of a register
        bool set_register(int address, data8 data) {
            return regs.set(address, data);
        }

        /// Get the value of a register
        bool get_register(int address, data8* pointer) {
            return regs.get(address, pointer);
        }

        /// Cause a kern panic. This indirectly halts the machine.
        void panic(int location) {
            set_flag(FLAG_PANIC, true);
            set_flag(FLAG_HALT, true);

            std::bitset<8> flags_bitset(flags);
            std::cout << prefix << "panic    "
                      << " location=0x" << full_length(int) << std::hex << location
                      << " flags=" << flags_bitset
                      << " address=0x" << full_length(int) << std::hex << current_address
                      << " stack=0x" << full_length(int) << std::hex << address_stack.size()
                      << " state_register=0x" << full_length(char) << std::hex << state_register.get()
                << std::endl;

        }

        /// Tick the clock once
        void clock() {
            array<byte, INSTR_FULL_SIZE> data = array<byte, INSTR_FULL_SIZE>();
            memory->read(current_address, &data[0], INSTR_FULL_SIZE);

            this->interpret(data);

            // Only increment the address if we didn't jump
            if (get_flag(FLAG_JUMP)) {
                set_flag(FLAG_JUMP, false);
            } else {
                current_address += INSTR_FULL_SIZE;
            }
        }

        /// Tick the clock the specified amount of times
        void clock (int num) {
            for (int i = 0; i < num; i++) {
                clock();
            }
        }
};