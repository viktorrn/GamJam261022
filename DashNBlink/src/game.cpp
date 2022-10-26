#include "game.h"

#include <iostream>
#include <chrono>
#include <thread>

#include "graphics.h"

bool s_running = false;

bool game_init()
{
	return graphics_init();
}

void game_run()
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

        graphics_prepare();

        // TODO: Render stuff

        graphics_present();

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

void game_clean_up()
{

}
