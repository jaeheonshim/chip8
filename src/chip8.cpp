#include "chip8.h"
#include "chip8_fontset.h"
#include <algorithm>
#include <iterator>
#include <chrono>
#include <cstring>

Chip8::Chip8() : rand_gen(std::chrono::system_clock::now().time_since_epoch().count()) {
    rand_byte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

void Chip8::initialize() {
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    for(int i{ 0 }; i < 16; ++i) {
        V[i] = 0;
    }

    std::memset(gfx, 0, sizeof(gfx));

    for(int i{ 0 }; i < 80; ++i) {
        memory[i] = chip8_fontset[i];
    }

    delay_timer = 0;
    sound_timer = 0;
}

void Chip8::clock_cycle() {
    opcode = (memory[pc]<<8) | memory[pc+1];
    pc += 2;
    
    int x{ (opcode & 0x0F00) >> 8 };
    int y{ (opcode & 0x00F0) >> 4 };
    unsigned char flag;

    switch(opcode & 0xF000) {
        case 0x0000:
            switch(opcode & 0x0FFF) {
                case 0x00E0:
                    std::memset(gfx, 0, sizeof(gfx));
                    draw_flag = true;
                    break;
                case 0x00EE:
                    --sp;
                    pc = stack[sp];
                    break;
                default:
                    // Not implemented
                    break;
            }
            break;
        case 0x1000:
            pc = opcode & 0xFFF;
            break;
        case 0x2000:
            stack[sp] = pc;
            sp++;
            pc = opcode & 0xFFF;
            break;
        case 0x3000:
            if (V[x] == (opcode & 0xFF)) {
                pc += 2;
            }
            break;
        case 0x4000:
            if (V[x] != (opcode & 0xFF)) {
                pc += 2;
            }
            break;
        case 0x5000:
            if (V[x] == V[y]) {
                pc += 2;
            }
            break;
        case 0x6000: // vx := NN
            V[x] = opcode & 0xFF;
            break;
        case 0x7000:
            V[x] += opcode & 0xFF;
            break;
        case 0x8000:
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
                    if (V[x] > 0xFF - V[y]) flag = 1;
                    else flag = 0;
                    V[x] = V[x] + V[y];
                    V[0xF] = flag;
                    break;
                case 0x5: // vx -= vy
                    if (V[x] < V[y]) flag = 0;
                    else flag = 1;
                    V[x] = V[x] - V[y];
                    V[0xF] = flag;
                    break;
                case 0x6: //
                    flag = V[x] & 0x1;
                    V[x] = V[x] >> 1;
                    V[0xF] = flag;
                    break;
                case 0x7:
                    if (V[x] > V[y]) flag = 0;
                    else flag = 1;
                    V[x] = V[y] - V[x];
                    V[0xF] = flag;
                    break;
                case 0xE:
                    flag = (V[x] & 0x80) >> 7;
                    V[x] = V[x] << 1;
                    V[0xF] = flag;
                    break;
            }
            break;
        case 0x9000:
            if (V[x] != V[y]) {
                pc += 2;
            }
            break;
        case 0xA000:
            I = opcode & 0xFFF;
            break;
        case 0xB000:
            pc = (opcode & 0xFFF) + V[0];
            break;
        case 0xC000:
            V[x] = rand_byte(rand_gen) & (opcode & 0xFF);
            break;
        case 0xD000:
            V[0xF] = 0;
            for(unsigned char r_i{ 0 }; r_i < (opcode & 0xF); ++r_i) {
                for(unsigned char c_i{ 0 }; c_i < 8; ++c_i) {
                    if((memory[I + r_i] & (0x80 >> c_i)) != 0) {
                        unsigned char& pixel = gfx[(r_i + V[y]) * 64 + V[x] + c_i];
                        if(pixel == 1) {
                            V[0xF] = 1;
                        }
                        pixel ^= 1;
                    }
                }
            }
            draw_flag = true;
            break;
        case 0xE000:
            switch(opcode & 0xFF) {
                case 0x9E:
                    if(key[V[x]]) {
                        pc += 2;
                    }
                    break;
                case 0xA1:
                    if(!key[V[x]]) {
                        pc += 2;
                    }
                    break;
            }
            break;
        case 0xF000:
            unsigned char value = V[x];
            switch(opcode & 0xFF) {
                case 0x07:
                    V[x] = delay_timer;
                    break;
                case 0x0A:
                    pc -= 2; // going to stay on this instruction for now
                    if(fx0a_state == OUT) {
                        // entering for first time
                        fx0a_state = IN;
                        
                        // clear buffer
                        for(int i{ 0 }; i < 16; ++i) {
                            fx0a_buffer[i] = 0;
                        }
                    }

                    for(unsigned char i{ 0 }; i < 16; ++i) {
                        if(key[i]) {
                            fx0a_buffer[i] = 1;
                        } else {
                            if(fx0a_buffer[i]) {
                                // a pressed key was released, we can continue
                                fx0a_state = OUT;
                                V[x] = i;
                                pc += 2;
                                break;
                            }
                        }
                    }

                    break;
                case 0x15:
                    delay_timer = V[x];
                    break;
                case 0x18:
                    sound_timer = V[x];
                    break;
                case 0x1E:
                    I += V[x];
                    break;
                case 0x29:
                    I = V[x];
                    break;
                case 0x33:
                    memory[I + 2] = value % 10;
                    value /= 10;
                    memory[I + 1] = value % 10;
                    value /= 10;
                    memory[I] = value % 10;
                    break;
                case 0x55:
                    for(int i{ 0 }; i <= x; ++i) {
                        memory[I++] = V[i];
                    }
                    break;
                case 0x65:
                    for(int i{ 0 }; i <= x; ++i) {
                        V[i] = memory[I++];
                    }
                    break;
            }
            break;
    }
}

void Chip8::load(std::istream& prog) {
    constexpr std::size_t START = 0x200;
    constexpr std::size_t MAX_SIZE = 4096 - START;

    prog.read(reinterpret_cast<char*>(memory + START), MAX_SIZE);

    pc = START;
}