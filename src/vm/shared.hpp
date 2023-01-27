#pragma once

#include <array>

// Awful idea to put this in the shared header but fuck it I'm lazy.
using namespace std;

#define byte unsigned char
#define uint unsigned int
#define ushort unsigned short
#define ulong unsigned long

#define data8 byte
#define data16 short
#define data32 int
#define data64 long

#include <iomanip>
#define full_length(type) std::setfill('0') << std::setw(sizeof(type)*2)

#include <type_traits>
#define inherits(self, parent) std::enable_if_t<std::is_base_of<parent,self>::value>* = nullptr

