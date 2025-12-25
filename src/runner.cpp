#include "runner.h"
#include <FL/Fl.H>

void Chip8Runner::step() {
    chip.clock_cycle();

    gui.registers->update(chip);

    if(chip.draw_flag) {
        chip.draw_flag = false;
        gui.display->update(chip);
        gui.display->redraw();
    }
}

void Chip8Runner::tick() {
    auto now = clock::now();
    std::chrono::duration<double> dt = now - last;
    last = now;

    accum += dt.count();

    const double cycle_hz = BASE_CPU_HZ * gui.controls->slider->value();

    while(accum >= 1.0 / TIMER_HZ) {
        accum -= 1.0 / TIMER_HZ;

        // how many cycles to run in this 60-th second slice
        cycle_accum += cycle_hz / TIMER_HZ;

        // run an integer number of cycles
        const int cycles = static_cast<int>(cycle_accum);
        cycle_accum -= cycles;

        for(int i{ 0 }; i < cycles; ++i) {
            chip.clock_cycle();
        }

        if(chip.delay_timer > 0) --chip.delay_timer;
        if(chip.sound_timer > 0) --chip.sound_timer;
    }

    gui.registers->update(chip);

    if(chip.draw_flag) {
        chip.draw_flag = false;
        gui.display->update(chip);
        gui.display->redraw();
    }

    if(r) {
        Fl::repeat_timeout(1.0 / 240.0, &Chip8Runner::tick_cb, this);
    }
}