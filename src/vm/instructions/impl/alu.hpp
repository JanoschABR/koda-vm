#pragma once
#include "../shared_internal.hpp"
#include "../../utils.hpp"

namespace instructions::alu {

    /// Add two 8-bit numbers
    instr add (instruction_t_params) {
        byte a = 0;
        owner->get_register(data[0], &a);
        byte b = 0;
        owner->get_register(data[1], &a);

        ushort result = ((ushort)a + b);
        owner->set_flag(FLAG_ALU_16, (result > 255));
        owner->set_register(data[2], result);
    }

    /// Subtract two 8-bit numbers
    instr sub (instruction_t_params) {
        byte a = 0;
        owner->get_register(data[0], &a);
        byte b = 0;
        owner->get_register(data[1], &a);

        ushort result = ((ushort)a - b);
        owner->set_flag(FLAG_ALU_16, (result > 255));
        owner->set_register(data[2], result);
    }

    /// Binary AND two 8-bit numbers
    instr binary_and (instruction_t_params) {
        byte a = 0;
        owner->get_register(data[0], &a);
        byte b = 0;
        owner->get_register(data[1], &a);

        owner->set_flag(FLAG_ALU_16, false);
        owner->set_register(data[2], (a & b));
    }

    /// Binary OR two 8-bit numbers
    instr binary_or (instruction_t_params) {
        byte a = 0;
        owner->get_register(data[0], &a);
        byte b = 0;
        owner->get_register(data[1], &a);

        owner->set_flag(FLAG_ALU_16, false);
        owner->set_register(data[2], (a | b));
    }

    /// Binary XOR two 8-bit numbers
    instr binary_xor (instruction_t_params) {
        byte a = 0;
        owner->get_register(data[0], &a);
        byte b = 0;
        owner->get_register(data[1], &a);

        owner->set_flag(FLAG_ALU_16, false);
        owner->set_register(data[2], (a ^ b));
    }

    /// Binary NOT an 8-bit number
    instr binary_not (instruction_t_params) {
        byte a = 0;
        owner->get_register(data[0], &a);

        owner->set_flag(FLAG_ALU_16, false);
        owner->set_register(data[2], ~a);
    }

    /// Shift Left an 8-bit number
    instr binary_shift_l (instruction_t_params) {
        byte val = 0;
        owner->get_register(data[0], &val);
        byte shift = 0;
        owner->get_register(data[1], &val);

        ushort result = ((ushort)val << shift);
        owner->set_flag(FLAG_ALU_16, (result > 255));
        owner->set_register(data[2], result);
    }

    /// Shift Right an 8-bit number
    instr binary_shift_r (instruction_t_params) {
        byte val = 0;
        owner->get_register(data[0], &val);
        byte shift = 0;
        owner->get_register(data[1], &val);

        ushort result = ((ushort)val >> shift);
        owner->set_flag(FLAG_ALU_16, (result > 255));
        owner->set_register(data[2], result);
    }
}