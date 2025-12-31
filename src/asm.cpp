#include "asm.h"
#include <queue>
#include <algorithm>

bool is_valid_opcode(unsigned short opcode) {
    switch(opcode & 0xF000) {
        case 0x0000:
        case 0x1000:
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
        case 0x9000:
        case 0xA000:
        case 0xB000:
        case 0xC000:
        case 0xD000:
            return true;
        case 0x8000:
            switch(opcode & 0x000F) {
                case 0x0:
                case 0x1:
                case 0x2:
                case 0x3:
                case 0x4:
                case 0x5:
                case 0x6:
                case 0x7:
                case 0xE:
                    return true;
            }
            break;
        case 0xE000:
            switch(opcode & 0xFF) {
                case 0x9E:
                case 0xA1:
                    return true;
            }
            break;
        case 0xF000:
            switch(opcode & 0xFF) {
                case 0x07:
                case 0x0A:
                case 0x15:
                case 0x18:
                case 0x1E:
                case 0x29:
                case 0x33:
                case 0x55:
                case 0x65:
                    return true;
            }
            break;
    }

    return false;
}

// Traverses possible jump paths using BFS to handle misaligned instructions
std::vector<AsmRow> disassemble(const Chip8& chip8) {
    std::vector<AsmRow> result;

    bool visited[4096]{};
    std::queue<unsigned short> q;

    q.push(0x200);

    while(!q.empty()) {
        unsigned short i{ q.front() };
        unsigned short opcode{ static_cast<unsigned short>((chip8.memory[i] << 8) | (chip8.memory[i + 1])) };
        q.pop();

        if(i > 0xFFF) continue;
        if(visited[i]) continue;

        visited[i] = true;
        // if(!is_valid_opcode(opcode)) continue;

        result.push_back({i, opcode});

        switch(opcode & 0xF000) {
            case 0x1000:
                q.push(opcode & 0xFFF);
                continue;
            case 0x2000:
                q.push(opcode & 0xFFF);
                break;
            case 0xB000:
                // dynamic jump... how to handle this?
                break;
        }

        if(i + 2 <= 0xFFF && !visited[i + 2]) {
            q.push(i + 2);
        }
    }

    std::sort(result.begin(), result.end());

    return result;
}