#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H

// Struct that holds window pointer, render pointers and 4 integers indicate 4 directions.
typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
	int up;
	int down;
	int left;
	int right;
} AppStruct;

// Struct that holds player's current position and texture pointer.
typedef struct {
	int x;
	int y;
	SDL_Texture* texture;
} PlayerStruct;

#endif // STRUCTS_H