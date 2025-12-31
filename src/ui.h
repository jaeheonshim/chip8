#pragma once

#include "chip8.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Grid.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/Fl_Table_Row.H>

#include <vector>
#include <string>

class Chip8Gui; // Forward decl
class KeyBox;

class Chip8Registers : public Fl_Group {
public:
    static const int ROW_H = 25;

    Chip8Registers(int x, int y);
    void update(const Chip8& chip8);

    void activate();
    void deactivate();

private:
    Fl_Input* V[16];
    Fl_Input* I;
    Fl_Input* pc;
    Fl_Input* sp;
};

class Chip8Timers : public Fl_Group {
public:
    static const int ROW_H = 25;

    Chip8Timers(int x, int y);
    void update(const Chip8& chip8);

    void activate();
    void deactivate();

private:
    Fl_Input* delay;
    Fl_Input* sound;
};

class Chip8Display : public Fl_Widget {
public:

    static constexpr int W = 64;
    static constexpr int H = 32;

    Chip8Display() : Fl_Widget(0, 0, 0, 0) {

    }

    ~Chip8Display() override {
        if(off_buffer) fl_delete_offscreen(off_buffer);
    }

    void draw() override;
    void resize(int x, int y, int w, int h) override;
    void update(const Chip8& chip8);

private:
    unsigned char gfx_buffer[64 * 32];
    Fl_Offscreen off_buffer = 0;
};

class SpeedMultiplierSlider : public Fl_Hor_Value_Slider {
public:
    using Fl_Hor_Value_Slider::Fl_Hor_Value_Slider;

    int format(char* buf) override {
        std::snprintf(buf, 64, "%.2fx", value());
        return 1;
    }
};

class Chip8Controls : public Fl_Flex {
public:
    Fl_Button* run_pause;
    Fl_Button* step;
    Fl_Value_Slider* slider;

    Chip8Controls();
};

class Chip8Keybinds : public Fl_Grid {
public:
    KeyBox* keyboxes[16];

    Chip8Keybinds(Chip8Gui* gui);

    int get_key_index(int k);

private:
    char keybinds[16] = {'1', '2', '3', '4',
                        'Q', 'W', 'E', 'R',
                        'A', 'S', 'D', 'F',
                        'Z', 'X', 'C', 'V'};
};

struct AsmRow {
    char addr[5];
    char bytes[5];
};

class Chip8DisasmTable : public Fl_Table_Row {
public:
    std::vector<AsmRow> prog_rows;

    Chip8DisasmTable();
    void load_prog(const Chip8& chip);
    void resize(int X, int Y, int W, int H) override;

private:
    void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) override;
};

class Chip8Gui : public Fl_Window {
public:
    Chip8Gui(int w, int h, Chip8& chip);
    ~Chip8Gui();
    int handle(int e) override;

    Chip8Display* display;
    Chip8Registers* registers;
    Chip8Timers* timers;
    Chip8Controls* controls;
    Chip8Keybinds* keybinds;
    Chip8DisasmTable* disasm_table;
    Chip8& chip;
};

class KeyBox : public Fl_Box {
public:
    KeyBox(int X, int Y, int W, int H, char key, int index, Chip8Gui* gui) : Fl_Box(X, Y, W, H), key(key), index(index), gui(gui) {
        box(FL_THIN_DOWN_BOX);
        labelsize(14);

        char label[2]{ key, '\0'};
        copy_label(label);
    }

    void draw() override;

private:
    int index;
    char key;
    Chip8Gui* gui;
};