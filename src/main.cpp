
#include "vm/interpreter.hpp"
#include "vm/instructions/impl/base.hpp"
#include "vm/utils.hpp"

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

    // Control Instructions
    map.add_instruction(0x0000, instructions::noop);
    map.add_instruction(0x0001, instructions::halt);
    map.add_instruction(0x0002, instructions::panic);

    // Primitive Data Instructions
    map.add_instruction(0x0100, instructions::increment_register);
    map.add_instruction(0x0101, instructions::decrement_register);
    map.add_instruction(0x0102, instructions::store_into_register);
    map.add_instruction(0x0103, instructions::memory_to_register);
    map.add_instruction(0x0104, instructions::register_to_memory);

    // State Register Instructions
    map.add_instruction(0x0110, instructions::write_state_register);
    map.add_instruction(0x0111, instructions::read_state_register);

    // Jump Instructions
    map.add_instruction(0x0200, instructions::jump);
    map.add_instruction(0x0201, instructions::jump_compare);

    // ALU Instructions
    map.add_instruction(0x0300, instructions::compare);
    map.add_instruction(0x0300, instructions::compare_x16);



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
