#include "sound.h"

// An 4 size array to hold all sounds by pointers.
static Mix_Chunk* sounds[SND_MAX];
// Hold the game main music by pointer.
static Mix_Music* music;

// Forward declaration
static void loadSounds(void);

/**
 * @brief Initialize all sounds and main game music with NULL.
*/
void initSounds()
{
	memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);
	music = NULL;

	loadSounds();
}

/**
 * @brief Load all in-game sounds.
*/
static void loadSounds()
{
	sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("Resources/sounds/player_fire_sound.ogg");
	sounds[SND_ENEMY_FIRE] = Mix_LoadWAV("Resources/sounds/enemy_fire_sound.ogg");
	sounds[SND_PLAYER_DIE] = Mix_LoadWAV("Resources/sounds/player_die_sound.ogg");
	sounds[SND_ENEMY_DIE] = Mix_LoadWAV("Resources/sounds/enemy_die_sound.ogg");
}

/**
 * @brief Load input music.
 * @param filename  A pointer of char indicates the input music's file name.
*/
void loadMusic(char* filename)
{
	// If current music exists, stop and free the current one.
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
	// Load music by specified filename.
	music = Mix_LoadMUS(filename);
}

/**
 * @brief Play the loaded music for loop times from beginning to end.
 * @param loop An integer indicates how many times the music will be played, -1 for forever.
*/
void playMusic(int loop)
{
	// Only support 0 or infinity time for now.
	Mix_PlayMusic(music, (loop) ? -1 : 0);
}

/**
 * @brief Play the specified sound forever on specified channel.
 * @param id An integer indicates the id of the sound to be played.
 * @param channel An integer indicates the sound channel.
*/
void playSound(int id, int channel)
{
	// Play for infinity times.
	Mix_PlayChannel(channel, sounds[id], 0);
}