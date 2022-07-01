#include "draw.h"

// Forward declarations.
static SDL_Texture* getTexture(char*);
static void storeTexture(char*, SDL_Texture*);

/**
 * @brief Prepare renderer to draw.
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
	// Try to retrieve the specified texture from storage.
	SDL_Texture* texturePtr;
	texturePtr = getTexture(filename);

	// If not stored before.
	if (texturePtr == NULL)
	{
		// Set the log category to be application log, priority to be info, message is "Loading file name".
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
		// Load input file, create surface and generate the texture.
		texturePtr = IMG_LoadTexture(app.renderer, filename);
		// Store the specified texture into storage.
		storeTexture(filename, texturePtr);
	}

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

/**
 * @brief Draws the SDL_Rect part of a specified texture on screen at the specified x and y coordinates.
 * @param texturePtr Input texture's pointer
 * @param srcPtr A pointer to a SDL_Rect object indicates the input texture's SDL_Rect object.
 * @param x An int indicates specified x
 * @param y An int indicates specified y
*/
void blitRect(SDL_Texture* texturePtr, SDL_Rect* srcPtr, int x, int y)
{
	// Set rect part's position and size.
	SDL_Rect dest;
	dest.h = srcPtr->h;
	dest.w = srcPtr->w;
	dest.x = x;
	dest.y = y;

	// Render input texture into dest with specified rect size.
	SDL_RenderCopy(app.renderer, texturePtr, srcPtr, &dest);
}

/**
 * @brief Get the pointer of SDL_Texture according to input filename.
 * @param name A pointer of char indicates the filename.
 * @return A pointer of SDL_Texture indicates the SDL_Texture we need.
*/
static SDL_Texture* getTexture(char* name)
{
	// Traverse the texture linked list and try to retrieve the specified one.
	for (TextureStruct* currTexturePtr = app.textureHead.next; currTexturePtr != NULL; currTexturePtr = currTexturePtr->next)
	{
		if (strcmp(currTexturePtr->name, name) == 0)
		{
			return currTexturePtr->texture;
		}
	}
	return NULL;
}

/**
 * @brief Store the input named SDL_Texture as a new TextureStruct object inside the TextureStruct linked list.
 * @param name A pointer of char indicates the name of input SDL_Texture object.
 * @param sdlTexture  A pointer of SDL_Texture indicates the input SDL_Texture object.
*/
static void storeTexture(char* name, SDL_Texture* sdlTexture)
{
	// Create and initialize a new TextureStruct object.
	TextureStruct* newTexture = malloc(sizeof(TextureStruct));
	memset(newTexture, 0, sizeof(TextureStruct));

	// Insert the newly created TextureStruct at the tail.
	app.textureTailPtr->next = newTexture;
	app.textureTailPtr = newTexture;

	// Set new TextureStruct's name the same as input and add tail null terminator.
	strncpy(newTexture->name, name, MAX_TEXTURE_NAME_LEN);
	newTexture->name[MAX_TEXTURE_NAME_LEN - 1] = '\0';

	// Set new TextureStruct's texture.
	newTexture->texture = sdlTexture;
}