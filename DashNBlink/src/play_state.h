#pragma once

#include "miniaudio.h"

void play_state_init(ma_engine* engine);

void play_state_run();

void play_sound(int soundIndex, ma_engine* engine);

void play_state_tick(float DeltaT, ma_engine* engine);

void play_state_draw();

void play_state_clean_up();
