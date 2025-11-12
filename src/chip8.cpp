#include "chip8.hpp"

#include <algorithm>
#include <iterator>

void Chip8::initialize() {
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;
}

void Chip8::clock_cycle() {
    opcode = (memory[pc]<<8) | memory[pc+1];
    pc += 2;
    
    int x, y;
    switch(opcode & 0xF000) {
        case 0x6000: // vx := NN
            V[(opcode & 0x0F00) >> 8] = opcode & 0xFF;
            break;
        case 0x7000:
            V[(opcode & 0x0F00) >> 8] += opcode & 0xFF;
            break;
        case 0x8000:
            x = (opcode & 0x0F00) >> 8;
            y = (opcode & 0x00F0) >> 4;
            switch(opcode & 0x000F) {
                case 0x0:
                    V[x] = V[y];
                    break;
                case 0x1:
                    V[x] = V[x] | V[y];
                    break;
                case 0x2:
                    V[x] = V[x] & V[y];
                    break;
                case 0x3:
                    V[x] = V[x] ^ V[y];
                    break;
                case 0x4:
                    if (V[x] > 0xFF - V[y]) V[0xF] = 1;
                    else V[0xF] = 0;
                    V[x] = V[x] + V[y];
                    break;
                case 0x5: // vx -= vy
                    if (V[x] < V[y]) V[0xF] = 0;
                    else V[0xF] = 1;
                    V[x] = V[x] - V[y];
                    break;
                case 0x6: //
                    V[0xF] = V[y] & 0x1;
                    V[x] = V[y] >> 1;
                    break;
                case 0x7:
                    if (V[x] > V[y]) V[0xF] = 0;
                    else V[0xF] = 1;
                    V[x] = V[y] - V[x];
                    break;
                case 0xE:
                    V[0xF] = V[y] & 0x8000;
                    V[x] = V[y] << 1;
                    break;
            }
            break;
    }
}

void Chip8::load(std::istream& prog) {
    std::istream_iterator<unsigned char> ii{ prog };
    std::copy_n(ii, 4096 - 512, memory + 512);
}