#include "chip8.h"
#include "ui.h"
#include "runner.h"
#include <fstream>
#include <chrono>

Chip8 chip8{};
Chip8Gui gui(1100, 650, chip8);
Chip8Audio audio{};
Chip8Runner runner(chip8, gui, audio);

void on_step(Fl_Widget* w, void* userdata) {
    runner.step();
}

void on_run_pause(Fl_Widget* w, void* userdata) {
    if(runner.running()) {
        gui.controls->run_pause->label("Run");
        gui.registers->activate();
        gui.timers->activate();
        runner.pause();
    } else {
        gui.controls->run_pause->label("Pause");
        gui.registers->deactivate();
        gui.timers->deactivate();
        runner.run();
    }
}

int main(int argc, char **argv) {
    chip8.initialize();

    gui.controls->run_pause->callback(on_run_pause);
    gui.controls->step->callback(on_step);

    std::ifstream ifs("breakout.ch8");
    chip8.load(ifs);
    gui.disasm_table->load_prog(chip8);

    return Fl::run();
}