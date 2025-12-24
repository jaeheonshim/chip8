#pragma once

#include "chip8.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Pack.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Button.H>

class Chip8Registers : public Fl_Group {
public:
    static const int ROW_H = 25;

    Chip8Registers(int x, int y);
    void update(const Chip8& chip8);

private:
    Fl_Input* V[16];
    Fl_Input* I;
    Fl_Input* pc;
    Fl_Input* sp;
};

class Chip8Display : public Fl_Widget {
public:
    static constexpr int W = 64;
    static constexpr int H = 32;

    Chip8Display(int x, int y, int w, int h) : Fl_Widget(x, y, w, h) {}

    void draw() override;
    void update(const Chip8& chip8);
private:
    unsigned char gfx_buffer[64 * 32];
};

class Chip8Controls : public Fl_Flex {
public:
    Fl_Button* step;

    Chip8Controls(int x, int y) : Fl_Flex(0, 0, 0, 30, Fl_Flex::ROW) {
        begin();

        new Fl_Button(0, 0, 0, 0, "Run");
        step = new Fl_Button(0, 0, 0, 0, "Step");
        new Fl_Button(0, 0, 0, 0, "Reset");

        end();
    }
};

class Chip8Gui : public Fl_Window {
public:
    Chip8Gui(int w, int h);
    ~Chip8Gui();

    Chip8Display* display;
    Chip8Registers* registers;
    Chip8Controls* controls;
};