#pragma once
#include "miniaudio.h"

class Chip8Audio {
public:
    Chip8Audio();
    ~Chip8Audio();

    void start();
    void stop();

private:
    static void data_cb(ma_device* dev, void* out, const void*, ma_uint32 frameCount);

    ma_device device{};
    bool playing = false;
    float phase = 0.0f;
};