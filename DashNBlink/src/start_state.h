#pragma once

#include "miniaudio.h"

void start_state_init(ma_engine* engine);

void start_state_run();

void start_state_tick(float DeltaT);

void start_state_draw();

void start_state_clean_up();
