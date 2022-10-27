#include "start_state.h"

#include "keyboard.h"
#include "game.h"

#include "graphics.h"

static ma_sound s_theme;

void start_state_init(ma_engine* engine)
{
	ma_result result = ma_sound_init_from_file(engine, "./res/audio/pdnd.mp3", 0, NULL, NULL, &s_theme);
	if (result != MA_SUCCESS) {
		return;
	}
}

void start_state_run()
{
	ma_sound_seek_to_pcm_frame(&s_theme, 0);
	ma_sound_start(&s_theme);
	ma_sound_set_looping(&s_theme, MA_TRUE);
}

void start_state_tick()
{
	if (keyboard_is_pressed(GLFW_KEY_SPACE))
	{
		ma_sound_stop(&s_theme);
		game_state_set(PLAY);
	}
}

void start_state_draw()
{
	graphics_prepare();

	graphics_present();
}

void start_state_clean_up()
{
	ma_sound_uninit(&s_theme);
}
