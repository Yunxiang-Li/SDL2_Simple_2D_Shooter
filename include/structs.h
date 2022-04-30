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
	int fire;
} AppStruct;

/* Struct that holds each entity's current position, current delta movement,
health points and texture pointer(simulation of inheritance).
*/
typedef struct {
	int x;
	int y;
	int dx;
	int dy;
	int health;
	SDL_Texture* texture;
} EntityStruct;

#endif // STRUCTS_H