#pragma once
#include "../shared.hpp"

/// The size of an instruction, in bytes
#define INSTR_FULL_SIZE 8/*24*/

/// The size of the instruction address, in bytes
#define INSTR_ADDR_SIZE 2

/// The size of the instruction body, in bytes
#define INSTR_DATA_SIZE (INSTR_FULL_SIZE - INSTR_ADDR_SIZE)

/// The max number of instructions
#define INSTR_COUNT 65536

