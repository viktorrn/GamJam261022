#include "game.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <iostream>

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    // In playback mode copy data to pOutput. In capture mode read data from pInput. In full-duplex mode, both
    // pOutput and pInput will be valid and you can move data from pInput into pOutput. Never process more than
    // frameCount frames.
}

static bool s_running = true;

int main()
{
    std::cout << "Soundengine starting\n" << std::endl;

    ma_result result;
    ma_context context;
    if (ma_context_init(NULL, 0, NULL, &context) != MA_SUCCESS) {
        std::cout << "Soundengine Context failed starting\n" << std::endl;
        return -2;
    }

    ma_device_info* pPlaybackInfos;
    ma_uint32 playbackCount;
    ma_device_info* pCaptureInfos;
    ma_uint32 captureCount;
    if (ma_context_get_devices(&context, &pPlaybackInfos, &playbackCount, &pCaptureInfos, &captureCount) != MA_SUCCESS) {
        std::cout << "Soundengine context get device failed\n" << std::endl;
    }

    // Loop over each device info and do something with it. Here we just print the name with their index. You may want
    // to give the user the opportunity to choose which device they'd prefer.
    for (ma_uint32 iDevice = 0; iDevice < playbackCount; iDevice += 1) {
        printf("%d - %s\n", iDevice, pPlaybackInfos[iDevice].name);
    }

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.pDeviceID = &pPlaybackInfos[0].id;
    config.playback.format = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
    config.playback.channels = 2;            // Set to 0 to use the device's native channel count.
    config.sampleRate = 0;                  // Set to 0 to use the device's native sample rate.
    config.dataCallback = data_callback;   // This function will be called when miniaudio needs more data.
    //config.pUserData = pMyCustomData;   // Can be accessed from the device object (device.pUserData).

    ma_device device;
    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
        return -1;  // Failed to initialize the device.
    }

    ma_device_start(&device);     // The device is sleeping by default so you'll need to start it manually.

   
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        return result;  // Failed to initialize the engine.
        std::cout << result << std::endl;
    }

    ma_sound startScreenTheme;
    result = ma_sound_init_from_file(&engine, "./res/audio/pdnd.mp3", 0, NULL, NULL, &startScreenTheme);
    if (result != MA_SUCCESS) {
        return result;
    }
    ma_sound mainTheme;
    result = ma_sound_init_from_file(&engine, "./res/audio/pdnu.mp3", 0, NULL, NULL, &mainTheme);
    if (result != MA_SUCCESS) {
        return result;
    }

    /*For audio documentation https://miniaud.io/docs/manual/index.html*/

    std::cout << "Game starting\n" << std::endl;

    if (!game_init())
    {
        std::cout << "\nFailed to initialize game, closing :(...\n" << std::endl;
        return 1;
    }

   

    std::cout << "\nRunning game...\n" << std::endl;

    game_run(&engine);

    game_clean_up();

    ma_sound_uninit(&startScreenTheme);
    ma_sound_uninit(&mainTheme);
    ma_device_uninit(&device);    // This will stop the device so no need to do that manually.
    ma_context_uninit(&context);
    std::cout << "\nGame closing\n" << std::endl;

    return 0;
}
