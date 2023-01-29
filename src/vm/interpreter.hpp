#pragma once

#include <stack>
#include <iostream>
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


class instruction_interpreter {
    protected:
        string prefix = "";

        instruction_map* map = nullptr;
        x8registers<16> regs;
        x8memory<MEMORY_SIZE>* memory;

        int flags = 0b0000000000000000;
        x8register state = x8register();

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

        inline void set_instruction_map (instruction_map* _map) {
            this->map = _map;
        }

        inline void set_prefix (const string& _prefix) {
            prefix = _prefix;
        }

        inline x8memory<MEMORY_SIZE>* get_memory () {
            return memory;
        }

        inline x8register* get_state_register () {
            return &state;
        }

        void jump(int address) {
            current_address = address;
        }

        void jump_onto_stack(int address) {
            address_stack.push(current_address);
            jump(address);
        }

        void jump_from_stack() {
            jump(address_stack.top());
            address_stack.pop();
        }

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

            // Check if we halted
            if (get_flag(FLAG_HALT)) {
                std::cout << prefix <<  "halt     "
                          << " address=0x" << full_length(int) << std::hex << current_address
                          << " flags=0x" << full_length(int) << std::hex << flags
                          << " state=0x" << full_length(char) << std::hex << (int)state.get()
                          << std::endl;
                return;
            }

            // Only report done if we haven't panicked
            if (!get_flag(FLAG_PANIC)) {
                std::cout << prefix <<  "ok       "
                          << " address=0x" << full_length(int) << std::hex << current_address
                          << " flags=0x" << full_length(int) << std::hex << flags
                          << " instruction=0x" << full_length(short) << std::hex << code
                          << " state=0x" << full_length(char) << std::hex << (int)state.get()
                          << std::endl;
            }
        }

        void clear_flags() {
            this->flags = 0;
        }

        void set_flag(int flag, bool value = true) {
            this->flags = ((this->flags & ~flag) | ((value ? 0xFFFF : 0x0000) & flag));
        }

        bool get_flag(int flag) {
            return (this->flags & flag);
        }

        bool set_register(int address, data8 data) {
            return regs.set(address, data);
        }

        bool get_register(int address, data8* pointer) {
            return regs.get(address, pointer);
        }

        void panic(int location) {
            set_flag(FLAG_PANIC, true);
            set_flag(FLAG_HALT, true);

            std::cout << prefix << "panic    "
                << " location=0x" << full_length(int) << std::hex << location
                << " flags=0x" << full_length(int) << std::hex << flags
                << " address=0x" << full_length(int) << std::hex << current_address
                << " stack=0x" << full_length(int) << std::hex << address_stack.size()
                << " state=0x" << full_length(char) << std::hex << state.get()
                << std::endl;

        }

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
};