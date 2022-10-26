#include "game.h"

#include <iostream>
#include <chrono>
#include <thread>

#include "GLFW/glfw3.h"

#include "graphics.h"
#include "start_state.h"
#include "play_state.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

static state s_current_state = START;

static ma_engine s_engine;
static ma_device_config s_config;
static ma_context s_context;
static ma_device s_device;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    // In playback mode copy data to pOutput. In capture mode read data from pInput. In full-duplex mode, both
    // pOutput and pInput will be valid and you can move data from pInput into pOutput. Never process more than
    // frameCount frames.
}

static room s_room;
static player s_player;
static float DeltaT = 0;

bool game_init()
{
    std::cout << "Soundengine starting\n" << std::endl;

    ma_result result;
    if (ma_context_init(NULL, 0, NULL, &s_context) != MA_SUCCESS) {
        std::cout << "Soundengine Context failed starting\n" << std::endl;
        return -2;
    }

    ma_device_info* pPlaybackInfos;
    ma_uint32 playbackCount;
    ma_device_info* pCaptureInfos;
    ma_uint32 captureCount;
    if (ma_context_get_devices(&s_context, &pPlaybackInfos, &playbackCount, &pCaptureInfos, &captureCount) != MA_SUCCESS) {
        std::cout << "Soundengine context get device failed\n" << std::endl;
    }

    // Loop over each device info and do something with it. Here we just print the name with their index. You may want
    // to give the user the opportunity to choose which device they'd prefer.
    for (ma_uint32 iDevice = 0; iDevice < playbackCount; iDevice += 1) {
        printf("%d - %s\n", iDevice, pPlaybackInfos[iDevice].name);
    }

    s_config = ma_device_config_init(ma_device_type_playback);
    s_config.playback.pDeviceID = &pPlaybackInfos[0].id;
    s_config.playback.format = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
    s_config.playback.channels = 2;            // Set to 0 to use the device's native channel count.
    s_config.sampleRate = 0;                  // Set to 0 to use the device's native sample rate.
    s_config.dataCallback = data_callback;   // This function will be called when miniaudio needs more data.
    //config.pUserData = pMyCustomData;   // Can be accessed from the device object (device.pUserData).

    if (ma_device_init(NULL, &s_config, &s_device) != MA_SUCCESS) {
        return -1;  // Failed to initialize the device.
    }

    ma_device_start(&s_device);     // The device is sleeping by default so you'll need to start it manually.

    result = ma_engine_init(NULL, &s_engine);
    if (result != MA_SUCCESS) {
        return result;  // Failed to initialize the engine.
        std::cout << result << std::endl;
    }

    /*For audio documentation https://miniaud.io/docs/manual/index.html*/

    if (!graphics_init())
    {
        return 0;
    }

    start_state_init(&s_engine);
    play_state_init(&s_engine);

    return 1;
}

bool s_running = false;

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

    start_state_run();

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
        DeltaT = delta_ms.count()/1000.0f;

        if (graphics_window_closed())
        {
            s_running = false;
        }
 
    }
}

void game_tick()
{
    switch (s_current_state)
    {
    case START:
        start_state_tick(DeltaT);
        break;
    case PLAY:
        play_state_tick(DeltaT);
        break;
    }
    player_tick(&s_player, DeltaT, &s_room);
}

void game_draw()
{
    switch (s_current_state)
    {
    case START:
        start_state_draw();
        break;
    case PLAY:
        play_state_draw();
        break;
    }
}

void game_clean_up()
{
    start_state_clean_up();
    play_state_clean_up();

    graphics_clean_up();

    ma_device_uninit(&s_device);
    ma_context_uninit(&s_context);
}

void game_state_set(state new_state)
{
    s_current_state = new_state;

    switch (s_current_state)
    {
    case START:
        start_state_run();
        break;
    case PLAY:
        play_state_run();
        break;
    }
}