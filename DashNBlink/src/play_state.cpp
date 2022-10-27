#include "play_state.h"

#include "graphics.h"
#include "tiles.h"
#include "player.h"
#include "game.h"

static room s_room;
static player s_player;

static ma_sound s_theme;

void play_state_init(ma_engine* engine)
{
	ma_result result = ma_sound_init_from_file(engine, "./res/audio/pdnu.mp3", 0, NULL, NULL, &s_theme);
	if (result != MA_SUCCESS) {
		return;
	}

	room_load(&s_room, "res/map.png", "res/lookup.png");
}

void play_state_run()
{
	ma_sound_seek_to_pcm_frame(&s_theme, 0);
	ma_sound_start(&s_theme);
	ma_sound_set_looping(&s_theme, MA_TRUE);

	player_load(&s_player, &s_room, 0);
}

void play_state_tick(float DeltaT)
{
	if (s_player.done)
	{
		player_load(&s_player, &s_room, 0);
	}

	else if (s_player.dead)
	{
		ma_sound_stop(&s_theme);
		game_state_set(START);
	}

	player_tick(&s_player, DeltaT, &s_room);
}

void play_state_draw()
{
	graphics_prepare();

	room_draw(&s_room);
	player_draw(&s_player);

	graphics_present();
}

void play_state_clean_up()
{
	ma_sound_uninit(&s_theme);
}
