#pragma once

#include <istream>
#include <random>

// Instruction FX0A should wait for a key to be pressed and then subsequently
// released. We can use a simple state machine for this
enum FX0A_State {
    IN,
    OUT
};

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

    unsigned int prog_size = 0;
    bool draw_flag = false;

    void initialize();
    void clock_cycle();
    void load(std::istream& prog);

    Chip8();
private:
    std::default_random_engine rand_gen;
    std::uniform_int_distribution<uint8_t> rand_byte;

    FX0A_State fx0a_state = OUT;
    unsigned char fx0a_buffer[16];
};