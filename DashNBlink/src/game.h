#pragma once

enum state
{
    START = 0, PLAY, END
};

extern bool game_init();

extern void game_run();

extern void game_tick();

extern void game_draw();

extern void game_clean_up();

extern void game_state_set(state new_state);
