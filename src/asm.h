#pragma once

#include "chip8.h"
#include <vector>

struct AsmRow {
    unsigned short addr;
    unsigned short opcode;

    bool operator<(const AsmRow& other) {
        return addr < other.addr;
    }
};

std::vector<AsmRow> disassemble(const Chip8& chip8);