// Include all defined variables here.

#pragma once
#ifndef DEFS_H
#define DEFS_H

// Define MIN and MAX functions.
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// Define the game screen width and height here.

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Define max possible keyboard key here.
#define MAX_KEYBOARD_KEYS 104

// Define player and its bullet's speed.
#define PLAYER_SPEED 5
#define PLAYER_BULLET_SPEED 20

// Define player original position
#define PLAYER_ORIGIN_X 100
#define PLAYER_ORIGIN_Y 100

// Define each frame's supposed miliseconds in 60 FPS situation.
#define MILISECOND_PER_FRAME 16

// Define the player's cooldown for next bullet.
#define PLAYER_BULLET_COOLDOWN 8

// Define bullet's side(player side or enemy side).
#define PLAYER_SIDE 0
#define ENEMY_SIDE  1

// Define enemy's bullet speed.
#define ENEMY_BULLET_SPEED 8

// Define the game FPS.
#define FPS 60

// Define the maximum stars' num inside the game background.
#define MAX_STAR_NUM 500

// Define the maximum number of sound channels of the game(should not be too high).
#define MAX_SOUND_CHANNEL_NUM 8

// This enum holds all sound channels.
enum
{
	CH_ANY = -1,
	CH_PLAYER,
	CH_ENEMY_FIRE
};

// This enum holds all 4 in-game sounds.
enum
{
	SND_PLAYER_FIRE,
	SND_ENEMY_FIRE,
	SND_PLAYER_DIE,
	SND_ENEMY_DIE,
	SND_MAX
};

// Define the maximum length of a in-game string.
#define MAX_LINE_LENGTH 1024

#endif // DEFS_H

