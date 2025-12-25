#include "chip8.h"
#include "ui.h"
#include "runner.h"
#include <fstream>
#include <chrono>

Chip8 chip8{};
Chip8Gui gui(700, 400);
Chip8Runner runner(chip8, gui);

void on_step(Fl_Widget* w, void* userdata) {
    runner.step();
}

void on_run_pause(Fl_Widget* w, void* userdata) {
    if(runner.running()) {
        gui.controls->run_pause->label("Run");
        runner.pause();
    } else {
        gui.controls->run_pause->label("Pause");
        runner.run();
    }
}

int main(int argc, char **argv) {
    chip8.initialize();

    gui.controls->run_pause->callback(on_run_pause);
    gui.controls->step->callback(on_step);

    std::ifstream ifs("4-flags.ch8");
    chip8.load(ifs);

    return Fl::run();
}