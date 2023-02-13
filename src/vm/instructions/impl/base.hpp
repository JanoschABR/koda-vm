#pragma once
#include "../shared_internal.hpp"
#include "../../utils.hpp"

namespace instructions {

    /// No operation
    instr noop (instruction_t_params) {}

    /// Set the halt flag
    instr halt (instruction_t_params) {
        owner->set_flag(FLAG_HALT);
    }

    /// Trigger a kern panic
    instr panic (instruction_t_params) {
        owner->panic(3);
    }



    /// Increment the specified register
    instr increment_register (instruction_t_params) {
        byte buffer;
        owner->get_register(data[0], &buffer);
        owner->set_register(data[0], buffer + 1);
    }

    /// Decrement the specified register
    instr decrement_register (instruction_t_params) {
        byte buffer;
        owner->get_register(data[0], &buffer);
        owner->set_register(data[0], buffer - 1);
    }

    /// Store a constant into the specified register
    instr store_into_register (instruction_t_params) {
        owner->set_register(data[0], data[1]);
    }

    /// Store the data from a place in memory in the specified register
    instr memory_to_register (instruction_t_params) {
        byte buffer;
        ushort memory_address = bin::bytes_to_short(&data[1], 0);
        owner->get_memory()->read(memory_address, &buffer);
        owner->set_register(data[0], buffer);
    }

    /// Store the specified register in memory
    instr register_to_memory (instruction_t_params) {
        byte buffer;
        ushort memory_address = bin::bytes_to_short(&data[1], 0);
        owner->get_register(data[0], &buffer);
        owner->get_memory()->write(memory_address, buffer);
    }

    /// Write the specified register to the state register
    instr write_state_register (instruction_t_params) {
        byte buffer;
        owner->get_register(data[0], &buffer);
        owner->get_state_register()->set(buffer);
    }

    /// Read the state register into the specified register
    instr read_state_register (instruction_t_params) {
        byte buffer;
        owner->get_register(data[0], &buffer);
        owner->get_state_register()->set(buffer);
    }


    /// Unconditional jump
    instr jump (instruction_t_params) {
        ushort address = bin::bytes_to_short(&data[0], 0);
        owner->set_flag(FLAG_JUMP);
        owner->jump(address);
    }

    /// Compare two 8-bit numbers stored in the specified registers
    instr compare (instruction_t_params) {
        byte reg_a = data[0];
        byte reg_b = data[1];

        byte a = 0;
        owner->get_register(reg_a, &a);
        byte b = 0;
        owner->get_register(reg_b, &b);

        int result;
        if (a == b) {
            result = COMPARE_EQUAL;
        } else {
            if (a > b) {
                result = COMPARE_MORE;
            } else {
                result = COMPARE_LESS;
            }
        }

        owner->get_compare_register()->set(result);
    }

    /// Compare two 16-bit numbers stored in the specified registers
    instr compare_x16 (instruction_t_params) {
        byte reg_a = data[0];
        byte reg_b = data[1];
        byte reg_c = data[2];
        byte reg_d = data[3];


        byte a = 0;
        owner->get_register(reg_a, &a);
        byte b = 0;
        owner->get_register(reg_b, &b);
        byte c = 0;
        owner->get_register(reg_c, &c);
        byte d = 0;
        owner->get_register(reg_d, &d);

        ushort val_a = bin::bytes_to_short(a, b);
        ushort val_b = bin::bytes_to_short(c, d);

        int result;
        if (val_a == val_b) {
            result = COMPARE_EQUAL;
        } else {
            if (val_a > val_b) {
                result = COMPARE_MORE;
            } else {
                result = COMPARE_LESS;
            }
        }

        owner->get_compare_register()->set(result);
    }

    /// Jump if the compare register equals the specified value
    instr jump_compare (instruction_t_params) {
        ushort address = bin::bytes_to_short(&data[1], 0);

        byte value = 0;
        owner->get_register(data[0], &value);

        if (value == owner->get_compare_register()->get()) {
            owner->set_flag(FLAG_JUMP);
            owner->jump(address);
        }
    }
}