
#include "vm/interpreter.hpp"
#include "vm/instructions/impl/base.hpp"
#include "vm/utils.hpp"
#include "vm/instructions/impl/alu.hpp"
#include "vm/instructions/impl/ext.hpp"

#include <iostream>
#include <fstream>
using namespace std;

int main() {
    std::cout << "INSTR_FULL_SIZE = " << std::dec << INSTR_FULL_SIZE << std::endl;
    std::cout << "INSTR_ADDR_SIZE = " << std::dec << INSTR_ADDR_SIZE << std::endl;
    std::cout << "INSTR_DATA_SIZE = " << std::dec << INSTR_DATA_SIZE << std::endl;
    std::cout << "INSTR_COUNT     = " << std::dec << INSTR_COUNT << std::endl;
    std::cout << std::endl;

    instruction_map map = instruction_map();
    using namespace instructions;

    // Control Instructions
    map.add_instruction(0x0000, base::noop);
    map.add_instruction(0x0001, base::halt);
    map.add_instruction(0x0002, base::panic);

    // Primitive Data Instructions
    map.add_instruction(0x0100, base::increment_register);
    map.add_instruction(0x0101, base::decrement_register);
    map.add_instruction(0x0102, base::store_into_register);
    map.add_instruction(0x0103, base::memory_to_register);
    map.add_instruction(0x0104, base::register_to_memory);

    // State Register Instructions
    map.add_instruction(0x0110, base::write_state_register);
    map.add_instruction(0x0111, base::read_state_register);

    // Stack Instructions
    map.add_instruction(0x0120, base::push_to_stack);
    map.add_instruction(0x0121, base::pop_from_stack);
    map.add_instruction(0x0122, base::peek_into_stack);

    // Jump Instructions
    map.add_instruction(0x0200, base::jump);
    map.add_instruction(0x0201, base::jump_compare);

    // ALU Instructions
    map.add_instruction(0x0300, base::compare);
    map.add_instruction(0x0301, base::compare_x16);
    map.add_instruction(0x0310, alu::add);
    map.add_instruction(0x0311, alu::sub);
    map.add_instruction(0x0312, alu::binary_and);
    map.add_instruction(0x0313, alu::binary_or);
    map.add_instruction(0x0314, alu::binary_not);
    map.add_instruction(0x0315, alu::binary_xor);
    map.add_instruction(0x0316, alu::binary_shift_l);
    map.add_instruction(0x0317, alu::binary_shift_r);

    // External Instructions
    map.add_instruction(0x1000, ext::platform_info);
    map.add_instruction(0x1001, ext::external_invoke);

    x8memory<MEMORY_SIZE> memory = x8memory<MEMORY_SIZE>();
    instruction_interpreter interpreter = instruction_interpreter(&memory, &map);

    streampos size;
    ifstream file (R"(D:\Projects\CLion\koda-vm\bytecode.bin)", ios::in | ios::binary | ios::ate);

    if (file.is_open()) {
        size = file.tellg();

        if (size > memory.get_data_size()) {
            std::cout << "MEMORY OVERFLOW\n" << std::endl;
            size = memory.get_data_size();
        }

        file.seekg (0, ios::beg);
        file.read ((char*)memory.get_data_pointer(), size);
        file.close();
    } else {
        std::cout << "FILE ERROR\n" << std::endl;
    }

    int max = 16;
    while (!interpreter.get_flag(FLAG_HALT)) {
        if (max-- <= 0) break;
        interpreter.clock();
    }
}
