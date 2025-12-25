#include "audio.h"
#include "miniaudio.h"
#include <cstring>


constexpr float SAMPLE_RATE = 44100.0f;
constexpr float FREQ = 440.0f;

Chip8Audio::Chip8Audio() {
    ma_device_config cfg = ma_device_config_init(ma_device_type_playback);
    cfg.playback.format = ma_format_f32;
    cfg.playback.channels = 1;
    cfg.sampleRate = (ma_uint32) SAMPLE_RATE;
    cfg.dataCallback = data_cb;
    cfg.pUserData = this;

    ma_device_init(nullptr, &cfg, &device);
}

Chip8Audio::~Chip8Audio() {
    ma_device_uninit(&device);
}

void Chip8Audio::start() {
    if(!playing) {
        playing = true;
        ma_device_start(&device);
    }
}

void Chip8Audio::stop() {
    if(playing) {
        playing = false;
        ma_device_stop(&device);
    }
}

void Chip8Audio::data_cb(ma_device* dev, void* out, const void*, ma_uint32 frames) {
    auto* self = static_cast<Chip8Audio*>(dev->pUserData);
    float* buffer = static_cast<float*>(out);

    if(!self->playing) {
        memset(buffer, 0, frames * sizeof(float));
        return;
    }

    for(ma_uint32 i = 0; i < frames; ++i) {
        buffer[i] = (self->phase < 0.5f) ? 0.25f : -0.25f;
        self->phase += FREQ / SAMPLE_RATE;
        if(self->phase >= 1.0f) self->phase -= 1.0f;
    }
}