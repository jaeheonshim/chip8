#include "ui.h"

#include <string>

inline Fl_Flex* create_register_row(const char* label, Fl_Input*& input_ptr) {
    auto* row = new Fl_Flex(0, 0, 0, 30, Fl_Flex::ROW);
    row->begin();

    auto* lab = new Fl_Box(0, 0, 0, 0);
    lab->labelcolor(FL_DARK_GREEN);
    lab->copy_label(label);
    lab->align(FL_ALIGN_INSIDE | FL_ALIGN_RIGHT);

    input_ptr = new Fl_Input(0, 0, 0, 0);
    
    row->end();
    row->fixed(lab, 32);

    return row;
}

Chip8Registers::Chip8Registers(int x, int y) : Fl_Group(x, y, 2000, 242) {
    begin();

    auto* header = new Fl_Box(0, 0, w(), 16, "Registers");
    header->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

    auto* inner = new Fl_Flex(0, 16, w(), h() - 16, Fl_Flex::ROW);
    inner->box(FL_ENGRAVED_BOX);
    inner->margin(8);
    inner->gap(2);

    // General Purpose Registers
    auto* gpr_1 = new Fl_Flex(Fl_Flex::COLUMN);
    for(int i{ 0 }; i < 8; ++i) {
        auto r = create_register_row(("V" + std::to_string(i)).c_str(), V[i]);
        gpr_1->fixed(r, ROW_H);
    }
    gpr_1->end();

    auto* gpr_2 = new Fl_Flex(Fl_Flex::COLUMN);
    for(int i{ 8 }; i < 16; ++i) {
        auto r = create_register_row(("V" + std::to_string(i)).c_str(), V[i]);
        gpr_2->fixed(r, ROW_H);
    }
    gpr_2->end();

    // Index Register, Program Counter, Stack Pointer
    auto* others = new Fl_Flex(Fl_Flex::COLUMN);
    auto row_I = create_register_row("I", I);
    others->fixed(row_I, ROW_H);
    auto row_pc = create_register_row("PC", pc);
    others->fixed(row_pc, ROW_H);
    auto row_sp = create_register_row("SP", sp);
    others->fixed(row_sp, ROW_H);
    others->end();

    inner->end();

    end();
}

void Chip8Registers::update(const Chip8& chip8) {
    for(int i{ 0 }; i < 16; ++i) {
        V[i]->value(static_cast<int>(chip8.V[i]));
    }
    I->value(static_cast<int>(chip8.I));
    pc->value(static_cast<int>(chip8.pc));
    sp->value(static_cast<int>(chip8.sp));
}

void Chip8Display::draw() {
    fl_color(FL_BLACK);
    fl_rectf(x(), y(), w(), h());
}

Chip8Gui::Chip8Gui(int w, int h) : Fl_Window(w, h, "CHIP-8") {
    begin();

    auto* root = new Fl_Flex(0, 0, w, h, Fl_Flex::ROW);

    constexpr int DISP_W = 128;
    constexpr int DISP_H = 64;
    
    display = new Chip8Display(0, 0, DISP_W, DISP_H);

    auto* right_pane = new Fl_Flex(0, 0, 0, 0, Fl_Flex::COLUMN);
    right_pane->margin(4);
    right_pane->gap(4);

    controls = new Chip8Controls(0, 0);
    right_pane->fixed(controls, controls->h());

    registers = new Chip8Registers(0, 0);
    right_pane->fixed(registers, registers->h());

    right_pane->end();

    root->end();

    end();
    show();
}

Chip8Gui::~Chip8Gui() {

}