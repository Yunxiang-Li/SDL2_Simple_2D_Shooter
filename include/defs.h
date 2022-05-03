// Include all defined variables here.

#pragma once
#ifndef DEFS_H
#define DEFS_H

// Define the game screen width and height here.

#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720

// Define max possible keyboard key here.
#define MAX_KEYBOARD_KEYS 104

// Define player and its bullet's speed.
#define PLAYER_SPEED          5
#define PLAYER_BULLET_SPEED   20

// Define player and bullet's size.
#define PLAYER_WIDTH 48
#define PLAYER_HEIGHT 46
#define BULLET_WIDTH 5
#define BULLET_HEIGHT 5

// Define player original position
#define PLAYER_ORIGIN_X 100
#define PLAYER_ORIGIN_Y 100

// Define each frame's supposed miliseconds in 60 FPS situation.
#define MILISECOND_PER_FRAME 16

// Define the player's cooldown for next bullet.
#define PLAYER_BULLET_COOLDOWN 8

#endif // DEFS_H

