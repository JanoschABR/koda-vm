#pragma once

#include "../../shared.hpp"
#include "../shared.hpp"

class instruction_interpreter;
#define instruction_t_params array<byte, INSTR_DATA_SIZE> data, instruction_interpreter* owner
typedef void (*instruction_t)(instruction_t_params);

class instruction_map {
    protected:
        array<instruction_t, INSTR_COUNT> map = array<instruction_t, INSTR_COUNT>();

    public:
        /// Returns true if the given address is valid
        bool is_valid_address (int address){
            return (address >= 0 && address < INSTR_COUNT);
        }

        /// Maps the instruction to the specified address. Returns false on failure.
        bool add_instruction (int address, instruction_t instruction){
            if (!is_valid_address(address)) return false;
            this->map[address] = instruction;
            return true;
        }

        /// Attempts to retrieve the instruction mapped to the specified address. Returns false on failed.
        bool lookup (int address, instruction_t* pointer){
            if (!is_valid_address(address)) return false;
            *pointer = this->map[address];
            return true;
        }
};
