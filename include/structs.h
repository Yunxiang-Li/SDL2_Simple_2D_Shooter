#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H

// Struct that holds window pointer and render pointer.
typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
} AppStruct;

// Struct that holds player's current position and texture pointer.
typedef struct {
	int x;
	int y;
	SDL_Texture* texture;
} PlayerStruct;

#endif // STRUCTS_H