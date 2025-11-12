#include "chip8.hpp"
#include <fstream>

Chip8 chip8;

int main(int argc, char **argv) {
    chip8.initialize();

    std::ifstream ifs("prog.bin");
    chip8.load(ifs);

    for(;;) {
        chip8.clock_cycle();
    }

    return 0;
}