#include "ui.h"

#include <FL/fl_draw.H>

#include <string>
#include <cstring>
#include <cctype>

static constexpr int grid_to_key[16] = {1, 2, 3, 12, 4, 5, 6, 13, 7, 8, 9, 14, 10, 0, 11, 15};
static constexpr int key_to_grid[16] = {13, 0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 14, 3, 7, 11, 15};

static const char* _8_opcodes[] = {"LD", "OR", "AND", "XOR", "ADD", "SUB", "SHR", "SUBN", 0, 0, 0, 0, 0, 0, "SHL", 0};
static const char* _opcodes[] = {0, "JP", "CALL", "SE", "SNE", "SE", "LD", "ADD", 0, "SNE", "LD", "JP", "RND", "DRW", 0, 0};

Fl_Flex* create_register_row(const char* label, Fl_Input*& input_ptr, int label_size = 36) {
    auto* row = new Fl_Flex(0, 0, 0, 30, Fl_Flex::ROW);
    row->begin();

    auto* lab = new Fl_Box(0, 0, 0, 0);
    lab->labelcolor(FL_DARK_GREEN);
    lab->copy_label(label);
    lab->align(FL_ALIGN_INSIDE | FL_ALIGN_RIGHT);

    input_ptr = new Fl_Input(0, 0, 0, 0);
    
    row->end();
    row->fixed(lab, label_size);

    return row;
}

void draw_asm_colored(int X, int Y, int W, int H, unsigned short opcode) {
    unsigned short nA, nX, nY, nB;
    nA = opcode >> 12;
    nX = (opcode & 0xF00) >> 8;
    nY = (opcode & 0xF0) >> 4;
    nB = (opcode & 0xF);

    int x = X + 4;
    int baseline = Y + H - 4;

    auto draw_token = [&](const char* tok, Fl_Color color) {
        fl_color(color);
        fl_draw(tok, x, baseline);
        x += (int) fl_width(tok);
    };

    switch(nA) {
        case 0x0:
            switch(opcode & 0x0FFF) {
                case 0x00E0:
                    draw_token("CLS", ASM_OPCODE_COLOR);
                    break;
                case 0x00EE:
                    draw_token("RET", ASM_OPCODE_COLOR);
                    break;
                default:
                    draw_token("SYS", ASM_OPCODE_COLOR);
                    break;
            }
            break;
        case 0x1:
            draw_token("JP", ASM_OPCODE_COLOR);
            break;
        case 0x2:
            draw_token("CALL", ASM_OPCODE_COLOR);
            break;
        case 0x3:
            draw_token("SE", ASM_OPCODE_COLOR);
            break;
        case 0x4:
            draw_token("SNE", ASM_OPCODE_COLOR);
            break;
        case 0x5:
            draw_token("SE", ASM_OPCODE_COLOR);
            break;
        case 0x6:
            draw_token("LD", ASM_OPCODE_COLOR);
            break;
        case 0x7:
            draw_token("ADD", ASM_OPCODE_COLOR);
            break;
        case 0x8: {
            const char* op = _8_opcodes[nB];
            if(op) {
                draw_token(op, ASM_OPCODE_COLOR);
            }
            break;
        } break;
        case 0x9:
            draw_token("SNE", ASM_OPCODE_COLOR);
            break;
        case 0xA:
            draw_token("LD", ASM_OPCODE_COLOR);
            break;
        case 0xB:
            draw_token("JP", ASM_OPCODE_COLOR);
            break;
        case 0xC:
            draw_token("RND", ASM_OPCODE_COLOR);
            break;
        case 0xD:
            draw_token("DRW", ASM_OPCODE_COLOR);
            break;
        case 0xE:
            switch(opcode & 0xFF) {
                case 0x9E:
                    draw_token("SKP", ASM_OPCODE_COLOR);
                    break;
                case 0xA1:
                    draw_token("SKNP", ASM_OPCODE_COLOR);
                    break;
            }
            break;
        case 0xF:
            switch(opcode & 0xFF) {
                case 0x07:
                    draw_token("LD", ASM_OPCODE_COLOR);
                    break;
                case 0x0A:
                    draw_token("LD", ASM_OPCODE_COLOR);
                    break;
                case 0x15:
                    draw_token("LD", ASM_OPCODE_COLOR);
                    break;
                case 0x18:
                    draw_token("LD", ASM_OPCODE_COLOR);
                    break;
                case 0x1E:
                    draw_token("ADD", ASM_OPCODE_COLOR);
                    break;
                case 0x29:
                    draw_token("LD", ASM_OPCODE_COLOR);
                    break;
                case 0x33:
                    draw_token("LD", ASM_OPCODE_COLOR);
                    break;
                case 0x55:
                    draw_token("LD", ASM_OPCODE_COLOR);
                    break;
                case 0x65:
                    draw_token("LD", ASM_OPCODE_COLOR);
                    break;
            }
            break;
    }
}

Chip8Registers::Chip8Registers(int x, int y) : Fl_Group(x, y, 2000, 242) {
    begin();

    auto* header = new Fl_Box(0, 0, w(), 16, "Registers");
    header->labelsize(13);
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

void Chip8Registers::activate() {
    for(int i{ 0 }; i < 16; ++i) {
        V[i]->activate();
    }
    I->activate();
    pc->activate();
    sp->activate();
}

void Chip8Registers::deactivate() {
    for(int i{ 0 }; i < 16; ++i) {
        V[i]->deactivate();
    }
    I->deactivate();
    pc->deactivate();
    sp->deactivate();
}

Chip8Timers::Chip8Timers(int x, int y) : Fl_Group(x, y, 10, 242) {
    begin();

    auto* header = new Fl_Box(0, 0, w(), 16, "Timers");
    header->labelsize(13);
    header->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

    auto* inner = new Fl_Flex(0, 20, w(), h() - 20, Fl_Flex::ROW);
    inner->box(FL_ENGRAVED_BOX);
    inner->margin(8);
    inner->gap(2);

    auto* timers = new Fl_Flex(Fl_Flex::COLUMN);
    auto* delay_p = create_register_row("DELAY", delay, 54);
    timers->fixed(delay_p, ROW_H);
    auto* sound_p = create_register_row("SOUND", sound, 54);
    timers->fixed(sound_p, ROW_H);
    timers->end();

    inner->end();

    end();
}

void Chip8Timers::activate() {
    delay->activate();
    sound->activate();
}

void Chip8Timers::deactivate() {
    delay->deactivate();
    sound->deactivate();
}

void Chip8Timers::update(const Chip8& chip8) {
    delay->value(static_cast<int>(chip8.delay_timer));
    sound->value(static_cast<int>(chip8.sound_timer));
}

Chip8Controls::Chip8Controls() : Fl_Flex(0, 0, 0, 55, Fl_Flex::COLUMN) {
    gap(2);
    begin();

    auto* row = new Fl_Flex(Fl_Flex::ROW);
    run_pause = new Fl_Button(0, 0, 0, 0, "Run");
    step = new Fl_Button(0, 0, 0, 0, "Step");
    new Fl_Button(0, 0, 0, 0, "Reset");
    row->end();

    slider = new SpeedMultiplierSlider(0, 0, 0, 0, nullptr);
    slider->range(0.25, 4);
    slider->step(0.25);
    slider->value(1);
    fixed(slider, 20);

    end();
}

void Chip8Display::draw() {
    if(!off_buffer) off_buffer = fl_create_offscreen(w(), h());

    fl_begin_offscreen(off_buffer);
    fl_color(FL_BLACK);
    fl_rectf(x(), y(), w(), h());

    int sx = w() / W;
    int sy = h() / H;
    int scale = std::max(1, std::min(sx, sy));

    fl_color(FL_WHITE);
    for(int y = 0; y < 32; ++y) {
        for(int x = 0; x < 64; ++x) {
            if(gfx_buffer[y * 64 + x]) {
                fl_rectf(
                    x * scale,
                    y * scale,
                    scale,
                    scale
                );
            }
        }
    }

    fl_end_offscreen();

    fl_copy_offscreen(x(), y(), w(), h(), off_buffer, 0, 0);
}

void Chip8Display::resize(int x, int y, int w, int h) {
    Fl_Widget::resize(x, y, w, h);
    if(off_buffer) fl_delete_offscreen(off_buffer);
    off_buffer = fl_create_offscreen(w, h);
    redraw();
}

void Chip8Display::update(const Chip8& chip8) {
    memcpy(gfx_buffer, chip8.gfx, 64 * 32);
}

Chip8Keybinds::Chip8Keybinds(Chip8Gui* gui) : Fl_Grid(0, 0, 30, 30) {
    layout(4, 4, 0, 1);
    begin();

    for(int i{ 0 }; i < 16; ++i) {
        keyboxes[i] = new KeyBox(0, 0, 0, 0, keybinds[i], grid_to_key[i], gui);
        widget(keyboxes[i], i / 4, i % 4);
    }

    show_grid(0);
    end();
}

int Chip8Keybinds::get_key_index(int k) {
    for(int i{ 0 }; i < 16; ++i) {
        if(keybinds[i] == toupper(k)) {
            return grid_to_key[i];
        }
    }

    return -1;
}

Chip8Gui::Chip8Gui(int w, int h, Chip8& chip) : Fl_Window(w, h, "CHIP-8"), chip(chip) {
    begin();

    auto* root = new Fl_Flex(0, 0, w, h, Fl_Flex::ROW);

    auto* left_pane = new Fl_Flex(0, 0, 0, 0, Fl_Flex::COLUMN);
    
    display = new Chip8Display();
    left_pane->fixed(display, 256);
    keybinds = new Chip8Keybinds(this);

    left_pane->end();

    root->fixed(left_pane, 512);

    disasm_table = new Chip8DisasmTable();

    auto* right_pane = new Fl_Flex(0, 0, 0, 0, Fl_Flex::COLUMN);
    right_pane->margin(4);
    right_pane->gap(4);

    controls = new Chip8Controls();
    right_pane->fixed(controls, controls->h());

    registers = new Chip8Registers(0, 0);
    right_pane->fixed(registers, registers->h());
    timers = new Chip8Timers(0, 0);

    right_pane->end();

    root->end();

    end();
    show();
}

Chip8Gui::~Chip8Gui() {

}

int Chip8Gui::handle(int e) {   
    if (e == FL_KEYDOWN || e == FL_KEYUP) {
        bool down = (e == FL_KEYDOWN);
        int key = Fl::event_key();
        int key_index = keybinds->get_key_index(key);
        if(key_index != -1) {
            chip.key[key_index] = down;
            keybinds->keyboxes[key_to_grid[key_index]]->redraw();
            return 1;
        }
    }

    return Fl_Window::handle(e);
}

Chip8DisasmTable::Chip8DisasmTable() : Fl_Table_Row(0, 0, 0, 0) {
    cols(3);
    col_header(0);
    row_header(0);

    hscrollbar->hide();
    hscrollbar->deactivate();
    
    rows(300);

    col_width(0, 60);
    col_width(1, 60);
    row_height_all(18);

    end();
}

void Chip8DisasmTable::load_prog(const Chip8& chip) {
    prog_rows.clear();
    prog_rows.reserve(chip.prog_size);

    for(unsigned int i{ 0 }; i < chip.prog_size; ++i) {
        unsigned short addr{ static_cast<unsigned short>(0x200 + (i << 1)) };
        unsigned short opcode{ static_cast<unsigned short>((chip.memory[addr] << 8) | (chip.memory[addr + 1])) };

        AsmRow row;

        std::sprintf(row.addr, "%04X", addr);
        std::sprintf(row.bytes, "%04X", opcode);
        row.opcode = opcode;

        prog_rows.push_back(row);
    }

    rows(chip.prog_size);
}

void Chip8DisasmTable::resize(int X, int Y, int W, int H) {
    Fl_Table_Row::resize(X, Y, W, H);

    int fixed = col_width(0) + col_width(1);

    col_width(2, w() - fixed - Fl::scrollbar_size() - 2);
}

void Chip8DisasmTable::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) {
    switch(context) {
        case CONTEXT_STARTPAGE:
            fl_font(FL_COURIER, 14);
            return;
        case CONTEXT_CELL: {
            AsmRow row{ prog_rows[R] };

            fl_push_clip(X, Y, W, H);
            fl_color(FL_WHITE);
            fl_rectf(X, Y, W, H);

            fl_color(FL_BLACK);

            if (C == 0) {
                fl_draw(row.addr, X + 4, Y + 15);
            } else if (C == 1) {
                fl_draw(row.bytes, X + 4, Y + 15);
            } else if (C == 2) {
                draw_asm_colored(X, Y, W, H, row.opcode);
            }
            
            fl_pop_clip();
        }
    }
}

void KeyBox::draw()  {
    if(gui->chip.key[index]) {
        color(FL_YELLOW);
    } else {
        color(FL_BACKGROUND_COLOR);
    }

    Fl_Box::draw();

    fl_font(FL_HELVETICA, 10);
    fl_color(FL_DARK3);

    char idx[2]{ hex[index], '\0' };
    fl_draw(idx, x() + 3, y() + 10);
}