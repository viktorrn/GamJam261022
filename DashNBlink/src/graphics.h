#pragma once

#include "tiles.h"

extern bool graphics_init();

extern void graphics_prepare();

extern void graphics_present();

extern void graphics_clean_up();

extern bool graphics_window_closed();

extern void graphics_tile_draw(const tile* t, float x, float y);
