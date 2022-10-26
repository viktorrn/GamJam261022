#include "game.h"

#include <iostream>
#include <chrono>
#include <thread>

#include "graphics.h"
#include "tiles.h"
#include "player.h"

static room s_room;
static player s_player;

bool game_init()
{
    if (!graphics_init())
    {
        return 0;
    }

    room_load(&s_room, "res/map.png", "res/lookup.png");
    player_load(&s_player, &s_room, 0);

    return 1;
}

bool s_running = false;

void game_run(ma_engine* engine)
{
    int frames = 0;
    double delta_time = 1000.0 / 90.0;

    std::chrono::system_clock::time_point last_frame = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> work_time;

    std::chrono::system_clock::time_point last_print = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> since_last_print;

    s_running = true;

    while (s_running)
    {
        last_frame = std::chrono::system_clock::now();

        game_tick();
        game_draw();

        frames++;

        since_last_print = current_time - last_print;

        if (since_last_print.count() > 1000.0f)
        {
            last_print = std::chrono::system_clock::now();
            std::cout << "frames: " << frames << std::endl;
            frames = 0;
        }

        current_time = std::chrono::system_clock::now();
        work_time = current_time - last_frame;

        std::chrono::duration<double, std::milli> delta_ms(delta_time - work_time.count());
        auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
        std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));

        if (graphics_window_closed())
        {
            s_running = false;
        }
 
    }
}

void game_tick()
{
    player_tick(&s_player);
}

void game_draw()
{
    graphics_prepare();

    room_draw(&s_room);
    player_draw(&s_player);

    graphics_present();
}

void game_clean_up()
{
    graphics_clean_up();
}
