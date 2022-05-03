// This class contains all used struct in the game.

#pragma once

#ifndef STRUCTS_H
#define STRUCTS_H

#include <defs.h>

// Forward declaration.
typedef struct EntityStruct_t EntityStruct;

// Struct that acts like a delegate of processing logic and render parts of the game.
typedef struct {
	void (*logic)(void);
	void (*draw)(void);
} DelegateStruct;

// Struct that holds window pointer, render pointers and 4 integers indicate 4 directions.
typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
	// Delegate of logic and render parts.
	DelegateStruct delegate;
	// Hold all possible keys' states.
	int keyboard[MAX_KEYBOARD_KEYS];
} AppStruct;

// Struct that holds each entity's attributes.
typedef struct EntityStruct_t {
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
	int reload;
	// Texture pointer
	SDL_Texture* texture;
	// Connected as a linked list node.
	EntityStruct* next;
}EntityStruct;

// Struct that hold information about fighters and bullets.
typedef struct {
	EntityStruct fighterHead;
	EntityStruct* fighterTail;
	EntityStruct bulletHead;
	EntityStruct* bulletTail;
} StageStruct;

#endif // STRUCTS_H