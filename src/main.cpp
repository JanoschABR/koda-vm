
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
    map.add_instruction(0x0000, instructions::noop);
    map.add_instruction(0x0001, instructions::halt);
    map.add_instruction(0x0002, instructions::panic);
    map.add_instruction(0x0100, instructions::jump);
    map.add_instruction(0x0101, instructions::jump_if_registers_equal);
    map.add_instruction(0x0102, instructions::jump_if_register_greater_than);
    map.add_instruction(0x0103, instructions::jump_if_register_less_than);

    /*processor<1> proc = processor<1>(&map);
    x8memory<MEMORY_SIZE>* memory = proc.get_memory();*/

    x8memory<MEMORY_SIZE> memory = x8memory<MEMORY_SIZE>();
    instruction_interpreter interpreter = instruction_interpreter(&memory, &map);

    /*interpreter.set_register(0, 6);
    interpreter.set_register(1, 6);
    interpreter.set_register(2, 8);

    // Write some bytecode into memory
    memory.write((INSTR_FULL_SIZE * 0) + 0, 0x01); // Jump Instruction (1st Byte)
    memory.write((INSTR_FULL_SIZE * 0) + 1, 0x00); // Jump Instruction (2nd Byte)
    memory.write((INSTR_FULL_SIZE * 0) + 2, 0x00); // Target Address (1st Byte)
    memory.write((INSTR_FULL_SIZE * 0) + 3, 0xFF); // Target Address (2nd Byte)

    memory.write(0x00FF + (INSTR_FULL_SIZE * 0) + 0, 0x01); // JIRG Instruction (1st Byte)
    memory.write(0x00FF + (INSTR_FULL_SIZE * 0) + 1, 0x02); // JIRG Instruction (2nd Byte)
    memory.write(0x00FF + (INSTR_FULL_SIZE * 0) + 2, 0x01); // Register A
    memory.write(0x00FF + (INSTR_FULL_SIZE * 0) + 3, 0x02); // Register B
    memory.write(0x00FF + (INSTR_FULL_SIZE * 0) + 4, 0x0F); // Target Address (1st Byte)
    memory.write(0x00FF + (INSTR_FULL_SIZE * 0) + 5, 0x00); // Target Address (2nd Byte)

    memory.write(0x00FF + (INSTR_FULL_SIZE * 1) + 0, 0x00); // Halt Instruction (1st Byte)
    memory.write(0x00FF + (INSTR_FULL_SIZE * 1) + 1, 0x01); // Halt Instruction (2nd Byte)

    memory.write(0x0F00 + (INSTR_FULL_SIZE * 1) + 0, 0x00); // Halt Instruction (1st Byte)
    memory.write(0x0F00 + (INSTR_FULL_SIZE * 1) + 1, 0x01); // Halt Instruction (2nd Byte)*/

    interpreter.set_register(0, 6);
    interpreter.set_register(1, 6);
    interpreter.set_register(2, 8);

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

    int max = 256;
    while (!interpreter.get_flag(FLAG_HALT)) {
        if (max-- <= 0) break;
        interpreter.clock();
    }
}
