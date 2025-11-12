#pragma once

#include <istream>

struct Chip8 {
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I;
    unsigned short pc;

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[16];
    unsigned short sp;

    unsigned char key[16];

    void initialize();
    void clock_cycle();
    void load(std::istream& prog);
};