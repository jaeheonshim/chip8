#include "chip8.h"
#include "ui.h"
#include <fstream>

Chip8 chip8{};

void on_step(Fl_Widget* w, void* userdata) {
    chip8.clock_cycle();
}

int main(int argc, char **argv) {
    chip8.initialize();

    Chip8Gui gui(700, 600);

    gui.controls->step->callback(on_step);

    std::ifstream ifs("3-corax+.ch8");
    chip8.load(ifs);

    while(Fl::wait()) {
        gui.registers->update(chip8);
        if(chip8.draw_flag) {
            gui.display->update(chip8);
            gui.display->redraw();
            chip8.draw_flag = false;
        }
    }


    return 0;
}