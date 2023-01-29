cmake_minimum_required(VERSION 3.24)
project(koda_vm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(koda_vm

        src/main.cpp
        src/vm/interpreter.hpp
        src/vm/instructions/shared.hpp
        src/vm/instructions/impl/base.hpp
        src/vm/memory/registers.hpp
        src/vm/shared.hpp
        src/vm/instructions/map/instruction_map.hpp
        src/vm/memory/memory.hpp
        src/vm/instructions/shared_internal.hpp
        src/vm/utils.hpp
        src/vm/specs.hpp)
