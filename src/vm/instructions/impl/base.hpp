#pragma once
#include "../shared_internal.hpp"
#include "../../utils.hpp"

namespace instructions {

    // Putting this here to make the code more neat inside the instruction definitions
    byte buffer;

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
        owner->get_register(data[0], &buffer);
        owner->set_register(data[0], buffer + 1);
    }

    /// Decrement the specified register
    instr decrement_register (instruction_t_params) {
        owner->get_register(data[0], &buffer);
        owner->set_register(data[0], buffer - 1);
    }

    /// Store a constant into the specified register
    instr store_into_register (instruction_t_params) {
        ushort constant = bin::bytes_to_short(&data[1], 0);
        owner->set_register(data[0], constant);
    }

    /// Store the data from a place in memory in the specified register
    instr memory_to_register (instruction_t_params) {
        ushort memory_address = bin::bytes_to_short(&data[1], 0);
        owner->get_memory()->read(memory_address, &buffer);
        owner->set_register(data[0], buffer);
    }

    /// Store the specified register in memory
    instr register_to_memory (instruction_t_params) {
        ushort memory_address = bin::bytes_to_short(&data[1], 0);
        owner->get_register(data[0], &buffer);
        owner->get_memory()->write(memory_address, buffer);
    }



    /// Unconditional jump
    instr jump (instruction_t_params) {
        ushort address = bin::bytes_to_short(&data[0], 0);
        owner->set_flag(FLAG_JUMP);
        owner->jump(address);
    }

    /// Jump if two registers' values equal
    instr jump_cond_registers_equal (instruction_t_params) {
        byte register_a = data[0];
        byte register_b = data[1];
        ushort address = bin::bytes_to_short(&data[2], 0);

        byte register_a_value = 0;
        owner->get_register(register_a, &register_a_value);
        byte register_b_value = 0;
        owner->get_register(register_b, &register_b_value);

        if (register_a_value == register_b_value) {
            owner->set_flag(FLAG_JUMP);
            owner->jump(address);
        }
    }

    /// Jump if the first register is greater than the second
    instr jump_cond_registers_greater (instruction_t_params) {
        byte register_a = data[0];
        byte register_b = data[1];
        ushort address = bin::bytes_to_short(&data[2], 0);

        byte register_a_value = 0;
        owner->get_register(register_a, &register_a_value);
        byte register_b_value = 0;
        owner->get_register(register_b, &register_b_value);

        if (register_a_value > register_b_value) {
            owner->set_flag(FLAG_JUMP);
            owner->jump(address);
        }
    }

    /// Jump if the first register is less than the second
    instr jump_cond_registers_less (instruction_t_params) {
        byte register_a = data[0];
        byte register_b = data[1];
        ushort address = bin::bytes_to_short(&data[2], 0);

        byte register_a_value = 0;
        owner->get_register(register_a, &register_a_value);
        byte register_b_value = 0;
        owner->get_register(register_b, &register_b_value);

        if (register_a_value < register_b_value) {
            owner->set_flag(FLAG_JUMP);
            owner->jump(address);
        }
    }
}