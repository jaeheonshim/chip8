#pragma once

#include "chip8.h"
#include "ui.h"
#include <chrono>

class Chip8Runner {
public:
    Chip8Runner(Chip8& c, Chip8Gui& g) : chip(c), gui(g) {
        last = clock::now();
    }

    void step(); // run a single cycle

    void run() {
        r = true;
        Fl::add_timeout(0.0, &Chip8Runner::tick_cb, this);
    }

    void pause() {
        r = false;
    }

    bool running() {
        return r;
    }

    static constexpr double TIMER_HZ = 60.0;
    static constexpr double BASE_CPU_HZ = 500.0;

private:
    using clock = std::chrono::steady_clock;

    static void tick_cb(void* userdata) {
        static_cast<Chip8Runner*>(userdata)->tick();
    }

    void tick(); // run cycles based on hz

    bool r = false;

    Chip8& chip;
    Chip8Gui& gui;

    clock::time_point last;
    double accum = 0.0;
    double cycle_accum = 0.0;
};