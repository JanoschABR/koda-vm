#pragma once
#include "../shared_internal.hpp"
#include "../../utils.hpp"

namespace instructions::ext {

    /// Stores a property of the platform info in the specified register
    instr platform_info (instruction_t_params) {
        static byte platform_info_properties[] {
                0x00, // Format Version

                0x01, // Platform Type
                0x00, // Platform ID

                // Platform Name (16 Bytes, ASCII Encoded)
                0x6B, 0x6F, 0x64, 0x61, 0x2D, 0x76, 0x6D, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                // Capability flags (i.e. Graphics, Peripherals, etc.)
                0b00000000, // Capability Flags (1st Byte)
                0b00000000, // Capability Flags (2nd Byte)
        };

        byte index = data[1];
        owner->set_register(data[0], platform_info_properties[index]);
    }

    /// Invoke something externally
    instr external_invoke (instruction_t_params) {
        // External Invocation does nothing on the VM as there is no Hardware to interact with

        std::cout << owner->get_prefix() << "external  0x"; // Print the data
        bin::print_hex(&data[0], INSTR_DATA_SIZE);
        std::cout << std::endl;
    }
}