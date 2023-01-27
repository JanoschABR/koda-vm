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

    /// Unconditional jump
    instr jump (instruction_t_params) {
        ushort address = bin::bytes_to_short(&data[0], 0);
        owner->set_flag(FLAG_JUMP);
        owner->jump(address);
    }

    /// Jump if two registers' values equal
    instr jump_if_registers_equal (instruction_t_params) {
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
    instr jump_if_register_greater_than (instruction_t_params) {
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

    /// Jump if the first register is greater than the second
    instr jump_if_register_less_than (instruction_t_params) {
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