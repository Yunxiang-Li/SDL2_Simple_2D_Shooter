#include "draw.h"

/**
 * @brief Prepare renderer to draw.
 *
 */
void prepareScene(void)
{
	// Set the color used for drawing operations(4 numbers for rgba).
	SDL_SetRenderDrawColor(appStruct.renderer, 96, 128, 255, 255);
	// Clear the current rendering target with the renderer's drawing color.
	SDL_RenderClear(appStruct.renderer);
}

/**
 * @brief Update the screen with backbuffer.
 *
 *
 */
void presentScene(void)
{
	SDL_RenderPresent(appStruct.renderer);
}