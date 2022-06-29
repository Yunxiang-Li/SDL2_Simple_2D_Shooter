#include "background.h"

// Declare an array to hold Star objects.
static StarStruct stars[MAX_STAR_NUM];

// Declare background's x position.
static int backgroundX;

// Declare the background's Texture.
static SDL_Texture* backgroundTexture;

/**
 * @brief Store background texture and initialize background's x position.
*/
void initBackground()
{
	backgroundTexture = loadTexture("Resources/images/background.png");
	backgroundX = 0;
}

/**
 * @brief Initialize 500 star objects at random position with random speed.
*/
void initStarfield()
{
	// Generate each star object's position and speed(this will also affect star's brightness).
	for (int i = 0; i < MAX_STAR_NUM; ++i)
	{
		stars[i].x = rand() % SCREEN_WIDTH;
		stars[i].y = rand() % SCREEN_HEIGHT;
		stars[i].speed = 1 + rand() % 8;
	}
}

/**
 * @brief Update the background's x position and check for corner case.
*/
void updateBackground()
{
	// Check the background's x's border case.
	if (++backgroundX > SCREEN_WIDTH)
	{
		backgroundX = 0;
	}
}

/**
 * @brief Update the whole star field(including every star object) linked list.
*/
void updateStarfield()
{
	// Iterate each star, if its x position is larger than the screen width, then reset it from the left side of the screen.
	for (int i = 0; i < MAX_STAR_NUM; ++i)
	{
		stars[i].x += stars[i].speed;

		if (stars[i].x > SCREEN_WIDTH)
		{
			// We want a little bit of offset here.
			stars[i].x = stars[i].x - SCREEN_WIDTH;
		}
	}
}

/**
 * @brief Draw all stars(each star is a line).
*/
void drawStarfield()
{
	int value;

	// Set each star's red, green and blue vaue and draw each star.
	for (int i = 0; i < MAX_STAR_NUM; ++i)
	{
		// Higher the speed of the star, brighter the color.
		value = 32 * stars[i].speed;

		SDL_SetRenderDrawColor(app.renderer, value, value, value, 255);

		// Each line is has distance of 3.
		SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
	}
}

/**
 * @brief Draw the game's whole background.
*/
void drawBackground()
{
	// A SDL_Rect object holds each background texture's size.
	SDL_Rect backgroundRect;

	// Sometimes more than one background may be drew.
	for (int x = backgroundX; x > -SCREEN_WIDTH; x -= SCREEN_WIDTH)
	{
		backgroundRect.x = x;
		backgroundRect.y = 0;
		backgroundRect.w = SCREEN_WIDTH;
		backgroundRect.h = SCREEN_HEIGHT;

		SDL_RenderCopy(app.renderer, backgroundTexture, NULL, &backgroundRect);
	}
}
