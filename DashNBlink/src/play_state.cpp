#include "play_state.h"

#include "graphics.h"
#include "tiles.h"
#include "player.h"
#include "game.h"
#include "keyboard.h"
#include <iostream>
#include <vector>

using namespace std;

static room s_room;
static player s_player;

static ma_sound s_theme;
static ma_sound s_start;

static ma_sound ps_death;
static ma_sound ps_land;
static ma_sound ps_leave;
static ma_sound ps_leaveRoom;

struct audioElement
{
	ma_sound soundFile;
	bool isPlaying = false;
};

vector<audioElement>audioVector;



void play_state_init(ma_engine* engine)
{
	ma_result result;

	result = ma_sound_init_from_file(engine, "./res/audio/pdnu.mp3", 0, NULL, NULL, &s_theme);
	if (result != MA_SUCCESS) {
		cout << "theme not found " << endl;
		return;
	}

	result = ma_sound_init_from_file(engine, "./res/audio/start_game.wav", 0, NULL, NULL, &s_start);
	if (result != MA_SUCCESS) {
		cout << "start game sound not found " << endl;
		return;
	}

	result = ma_sound_init_from_file(engine, "./res/audio/leave_room.wav", 0, NULL, NULL, &ps_leaveRoom);
	if (result != MA_SUCCESS) {
		cout << "leave room not found " << endl;
		return;
	}

	result = ma_sound_init_from_file(engine, "./res/audio/player_death.mp3", 0, NULL, NULL, &ps_death);
	if (result != MA_SUCCESS) {
		cout << "death sound not found " << endl;
		return;
	}

	result = ma_sound_init_from_file(engine, "./res/audio/player_land_platform.mp3", 0, NULL, NULL, &ps_land);
	if (result != MA_SUCCESS) {
		cout << "land sound not found " << endl;
		return;
	}

	result = ma_sound_init_from_file(engine, "./res/audio/player_leave_platform.mp3", 0, NULL, NULL, &ps_leave);
	if (result != MA_SUCCESS) {
		cout << "leave platform sound not found " << endl;
		return;
	}

}

/*
sound index table
0: start game  
1: leave room
2: death
3: land 
4: leave
5
6

*/

void play_sound(int soundIndex,ma_engine* engine)
{
	//ma_sound ss;
	audioElement sound;
	//ma_result result;
	cout << "play sound " << soundIndex << endl;
	switch (soundIndex)
	{
	case 0: 
		
		ma_sound_seek_to_pcm_frame(&s_start, 0);
		ma_sound_start(&s_start);
		break;
	case 1:
		
		ma_sound_seek_to_pcm_frame(&ps_leaveRoom, 0);
		ma_sound_start(&ps_leaveRoom);
		break;
	case 2:
		ma_sound_set_volume(&ps_death, 5.0f);
		ma_sound_seek_to_pcm_frame(&ps_death, 0);
		ma_sound_start(&ps_death);
		break;
	case 3:
		ma_sound_set_volume(&ps_land, 5.0f);
		ma_sound_seek_to_pcm_frame(&ps_land, 0);
		ma_sound_start(&ps_land);
	case 4:
		ma_sound_set_volume(&ps_leave, 5.5f);
		ma_sound_seek_to_pcm_frame(&ps_leave, 0);
		ma_sound_start(&ps_leave);
		break;
	
	}
	/*switch (soundIndex)
	{
	case 0:
		result = ma_sound_init_from_file(engine, "./res/audio/start-game.mp3", 0, NULL, NULL, &ss);
		if (result != MA_SUCCESS) {
			cout << "start game sound not found " << endl;
			return;
		}
		sound.soundFile = ss;
		audioVector.push_back(sound);
		break;
	case 1:
		result = ma_sound_init_from_file(engine, "./res/audio/leave-room.mp3", 0, NULL, NULL, &ss);
		if (result != MA_SUCCESS) {
			cout << "leave room not found " << endl;
			return;
		}
		sound.soundFile = ss;
		audioVector.push_back(sound);
		break;
		
	case 2:
		result = ma_sound_init_from_file(engine, "./res/audio/player-death.mp3", 0, NULL, NULL, &ss);
		if (result != MA_SUCCESS) {
			cout << "death sound not found " << endl;
			return;
		}
		sound.soundFile = ss;
		audioVector.push_back(sound);
		break;
	case 3:
		result = ma_sound_init_from_file(engine, "./res/audio/player-land-platform.mp3", 0, NULL, NULL, &ss);
		if (result != MA_SUCCESS) {
			cout << "land sound not found " << endl;
			return;
		}
		sound.soundFile = ss;
		audioVector.push_back(sound);
		break;
	case 4:
		result = ma_sound_init_from_file(engine, "./res/audio/pdnu.mp3", 0, NULL, NULL, &ss);
		if (result != MA_SUCCESS) {
			cout << "leave platform sound not found " << endl;
			return;
		}
		sound.soundFile = ss;
		audioVector.push_back(sound);
		break;
	}
	*/
}

void play_state_run()
{
	ma_sound_seek_to_pcm_frame(&s_theme, 0);
	ma_sound_start(&s_theme);
	ma_sound_set_looping(&s_theme, MA_TRUE);

	room_load(&s_room, "res/map", "res/lookup.png", 0);
	player_load(&s_player, &s_room, 0);
}



void play_state_tick(float DeltaT, ma_engine *engine)
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

	player_tick(&s_player, DeltaT, &s_room, engine);

	for ( int i = 0; i < audioVector.size(); i++)
	{
		audioElement sound = audioVector.at(i);

		if (ma_sound_at_end(&sound.soundFile))
		{
			audioVector.erase(audioVector.begin()+i);
			ma_sound_uninit( &sound.soundFile);
			continue;

		}
		
		if (!sound.isPlaying)
		{
			sound.isPlaying = true;
			ma_sound_seek_to_pcm_frame(&sound.soundFile, 0);
			ma_sound_start(&sound.soundFile);
		}
		
	}
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
