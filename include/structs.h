// This class contains all used struct in the game.

#pragma once

#ifndef STRUCTS_H
#define STRUCTS_H

#include <defs.h>

// Forward declaration.
typedef struct EntityStruct EntityStruct;
typedef struct ExplosionStruct ExplosionStruct;
typedef struct DebrisStruct DebrisStruct;
typedef struct DelegateStruct DelegateStruct;
typedef struct AppStruct AppStruct;
typedef struct StageStruct StageStruct;
typedef struct StarStruct StarStruct;
typedef struct HighscoreStruct HighScoreStruct;
typedef struct HighScoreTableStruct HighscoreTableStruct;
typedef struct TextureStruct TextureStruct;

// Struct that acts like a delegate of processing logic and render parts of the game.
struct DelegateStruct
{
	void (*logic)(void);
	void (*draw)(void);
};

// Struct that holds the name of the texture, a point to related SDL_Texture, and a pointer to the next TextureStruct in the linked list.
struct TextureStruct
{
	char name[MAX_TEXTURE_NAME_LEN];
	SDL_Texture* texture;
	TextureStruct* next;
};

// Struct that holds window pointer, render pointers, delegate struct, keyboard states and texture linked list head and tail pointer.
struct AppStruct 
{
	SDL_Renderer* renderer;
	SDL_Window* window;
	// Delegate of logic and render parts.
	DelegateStruct delegate;
	// Hold all possible keys' states.
	int keyboard[MAX_KEYBOARD_KEYS];
	// The head and the tail pointer of the texture linked list.
	TextureStruct textureHead;
	TextureStruct* textureTailPtr;
};

// Struct that holds each entity's attributes.
struct EntityStruct 
{
	// Current position
	float x;
	float y;
	// Current width and height.
	int width;
	int height;
	// Current delta movement
	float dx;
	float dy;
	// Current health points
	int health;
	// Track the player's ability to fire.
	int bulletCooldown;
	// Track down this object's side(player side or enemy side).
	int side;
	// Texture pointer
	SDL_Texture* texture;
	// Connected as a linked list node.
	EntityStruct* next;
};

// Struct that hold details of an explosion.
struct ExplosionStruct
{
	float x;
	float y;
	float dx;
	float dy;
	int red, green, blue, alpha;
	ExplosionStruct* next;
};

// Struct that hold debris when a ship is destroyed.
struct DebrisStruct
{
	float x;
	float y;
	float dx;
	float dy;
	SDL_Rect rect;
	SDL_Texture* texture;
	int life;
	DebrisStruct* next;
};

// Struct that hold information about shooters, bullets, explosions, debris, pointPod and score.
struct StageStruct 
{
	EntityStruct shooterHead;
	EntityStruct* shooterTailPtr;
	EntityStruct bulletHead;
	EntityStruct* bulletTailPtr;
	ExplosionStruct explosionHead;
	ExplosionStruct* explosionTailPtr;
	DebrisStruct debrisHead;
	DebrisStruct* debrisTailPtr;
	EntityStruct pointPodHead;
	EntityStruct* pointPodTailPtr;
	int score;
};

// Struct that holds details about stars inside the game background.
struct StarStruct 
{
	int x;
	int y;
	int speed;
};

// Struct that holds details of each highscore.
struct HighscoreStruct
{
	// Used for highlighting the score
	int recent;

	int score;
};

// Struct that holds all highscores inside an array of HighScoreStruct.
struct HighScoreTableStruct
{
	HighScoreStruct highscore[HIGHSCORE_TABLE_ROW_NUM];
};

#endif // STRUCTS_H