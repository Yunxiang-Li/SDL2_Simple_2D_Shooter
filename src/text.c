#include "text.h"
// A pointer of SDL_Texture to hold the font texture.
static SDL_Texture* fontTexture;
// A char array indicates the in-game text buffer.
static char drawTextBuffer[MAX_LINE_LENGTH];

/**
 * @brief Load the in-game font.
*/
void initFonts()
{
	fontTexture = loadTexture("Resources/fonts/font.png");
}

/**
 * @brief Draw texts with additional color and formatting at specified position.
 * @param x An integer indicates the x position where we draw our text at.
 * @param y An integer indicates the y position where we draw our text at.
 * @param r An integer indicates the red value of our drawing color.
 * @param g An integer indicates the green value of our drawing color.
 * @param b An integer indicates the blue value of our drawing color.
 * @param textPtr An pointer of char indicates the input text.
 * @param ... Additionally parameters for text formatting.
*/
void drawText(int x, int y, int r, int g, int b, char* textPtr, ...)
{
	// Clear the buffer.
	memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

	// Initializes args to retrieve the additional arguments after parameter format.
	va_list args;
	va_start(args, textPtr);
	// Format input string and store them into the buffer.
	vsprintf_s(drawTextBuffer, MAX_LINE_LENGTH, textPtr, args);
	// End using variable argument list.
	va_end(args);

	// Get the buffer's length.
	int len = strlen(drawTextBuffer);

	// Set each letter's related SDL_Rect.
	SDL_Rect rect;
	rect.w = LETTER_WIDTH;
	rect.h = LETTER_HEIGHT;
	rect.y = 0;

	// Set additional color for font texture.
	SDL_SetTextureColorMod(fontTexture, r, g, b);

	// Draw each letter.
	for (int i = 0; i < len; ++i)
	{
		int c = drawTextBuffer[i];

		// We only support from char space to char Z for now(due to font picture).
		if (c >= ' ' && c <= 'Z')
		{
			// Set up each letter's SDL_Rect's x position.
			rect.x = (c - ' ') * LETTER_WIDTH;

			blitRect(fontTexture, &rect, x, y);

			x += LETTER_WIDTH;
		}
	}
}