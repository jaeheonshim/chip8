#pragma once

#include <istream>
#include <random>

class Chip8 {
public:
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I;
    unsigned short pc;
    unsigned char gfx[64 * 32];

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[16];
    unsigned short sp;

    unsigned char key[16];

    void initialize();
    void clock_cycle();
    void load(std::istream& prog);

    Chip8();

private:
    std::default_random_engine rand_gen;
    std::uniform_int_distribution<uint8_t> rand_byte;
};