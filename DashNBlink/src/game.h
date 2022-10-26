#pragma once
#include "miniaudio.h"


extern bool game_init();

extern void game_run(ma_engine *engine);

extern void game_tick();

extern void game_draw();

extern void game_clean_up();
