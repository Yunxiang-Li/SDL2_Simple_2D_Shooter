#include "draw.h"

/**
 * @brief Prepare renderer to draw.
 *
 */
void prepareScene(void)
{
	// Set the color used for drawing operations(4 numbers for rgba).
	SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
	// Clear the current rendering target with the renderer's drawing color(happen only in backbuffer, cannot see yet).
	SDL_RenderClear(app.renderer);
}

/**
 * @brief Update the screen with backbuffer.
 *
 *
 */
void presentScene(void)
{
	SDL_RenderPresent(app.renderer);
}

/**
 * @brief Load the input file and return the generated SDL_Texture.
 * @param filename A C-sytle string indicates the filename.
 * @return A pointer to generated SDL_Texture object.
*/
SDL_Texture* loadTexture(char* filename)
{
	SDL_Texture* texturePtr;

	// Set the log category to be application log, priority to be info, message is "Loading file name".
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	// Load input file, create surface and generate the texture.
	texturePtr = IMG_LoadTexture(app.renderer, filename);

	return texturePtr;
}

/**
 * @brief Draws the specified texture on screen at the specified x and y coordinates.
 * @param texturePtr Input texture's pointer
 * @param x An int indicates specified x
 * @param y An int indicates specified y
*/
void blit(SDL_Texture* texturePtr, int x, int y)
{
	// Initialize the dest SDL_Rect object.
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;

	// Query the attributes of a texture(set dest SDL_Rect object's width and height).
	if (SDL_QueryTexture(texturePtr, NULL, NULL, &dest.w, &dest.h) < 0)
	{
		printf("Couldn't get texture's attributes: %s\n", SDL_GetError());
		exit(1);
	}
	
	// Render input texture into dest(SDL_Rect dest), NULL means render the full texture.
	SDL_RenderCopy(app.renderer, texturePtr, NULL, &dest);
}