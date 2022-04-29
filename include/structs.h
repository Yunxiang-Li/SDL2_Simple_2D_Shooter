#pragma once

// Struct that holdes window pointer and render pointer.
typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
} AppStruct;