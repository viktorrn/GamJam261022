#include "play_state.h"

#include "graphics.h"
#include "tiles.h"
#include "player.h"
#include "game.h"
#include "keyboard.h"

static room s_room;
static player s_player;

static ma_sound s_theme;

void play_state_init(ma_engine* engine)
{
	ma_result result = ma_sound_init_from_file(engine, "./res/audio/pdnu.mp3", 0, NULL, NULL, &s_theme);
	if (result != MA_SUCCESS) {
		return;
	}
}

void play_state_run()
{
	ma_sound_seek_to_pcm_frame(&s_theme, 0);
	ma_sound_start(&s_theme);
	ma_sound_set_looping(&s_theme, MA_TRUE);

	room_load(&s_room, "res/map", "res/lookup.png", 0);
	player_load(&s_player, &s_room, 0);
}

void play_state_tick(float DeltaT)
{
	if (keyboard_is_pressed(GLFW_KEY_R))
	{
		room_revert_all(&s_room);
		player_load(&s_player, &s_room, 0);
	}

	if (s_player.done)
	{
		int last_index = s_room.index;
		room_load_next(&s_room);

		if (last_index == s_room.index)
		{
			ma_sound_stop(&s_theme);
			game_state_set(START);
		}

		else
		{
			player_load(&s_player, &s_room, 0);
		}
	}

	else if (s_player.dead)
	{
		room_revert_all(&s_room);
		player_load(&s_player, &s_room, 0);
	}

	player_tick(&s_player, DeltaT, &s_room);
}

void play_state_draw()
{
	graphics_prepare();

	graphics_play_draw();

	room_draw(&s_room);
	player_draw(&s_player);

	graphics_present();
}

void play_state_clean_up()
{
	ma_sound_uninit(&s_theme);
}
