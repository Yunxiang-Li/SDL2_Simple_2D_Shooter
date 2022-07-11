#include "title.h"

// Forward function declarations.
static void titleScreenLogic(void);
static void titleScreenDraw(void);
static void drawLogo(void);

// Hold pointers of two titlescreen's captions' textures.
static SDL_Texture* sdl2CaptionTexture;
static SDL_Texture* shooterCaptionTexture;
// An integer indicates time interval between the title screen and the highscore table scene.
static int timeout;
// An integer indicates one logo's partial height value.
static int logoPartialHeight = 0;

/**
 * @brief Switch to the titlescreen.
*/
void switchToTitleScreen()
{
	// Assign our delegate function pointers to the app.
	app.delegate.logic = titleScreenLogic;
	app.delegate.draw = titleScreenDraw;

	// Reset keyboard states.
	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

	// Load two textures.
	sdl2CaptionTexture = loadTexture("Resources/images/SDL2_Captions.png");
	shooterCaptionTexture = loadTexture("Resources/images/SHOOTER_Captions.png");
	// Set time interval to be 5 seconds.
	timeout = FPS * 5;
}

/**
 * @brief A bound callback to handle all titlescreen updates.
*/
static void titleScreenLogic()
{
	// Update background and starfield.
	updateBackground();
	updateStarfield();

	// Increase logoPartialHeight is it is less than screen height.
	if (logoPartialHeight < SCREEN_HEIGHT)
	{
		++logoPartialHeight;
	}

	// Switch to highscore scene after 5 seconds under 60 fps condition.
	if (--timeout <= 0)
	{
		switchToHighscoreScene();
	}
	// If user press X key, switch to the stage(main game scene).
	if (app.keyboard[SDL_SCANCODE_X])
	{
		switchToStage();
	}
}

/**
 * @brief Draw the titlescreen.
*/
static void titleScreenDraw()
{
	// Draw background and starfield.
	drawBackground();
	drawStarfield();
	// Draw two titlescreen logos.
	drawLogo();

	// Blink the helper message per 10 frames.
	if (timeout % 20 < 10)
	{
		// Draw the helper message
		drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, TEXT_CENTER, "PRESS X KEY TO PLAY!");
	}
}

/**
 * @brief Draw two titlescreen logos.
*/
static void drawLogo()
{
	// A SDL_Rect object to help draw two logos.
	SDL_Rect tempRect;
	tempRect.x = 0;
	tempRect.y = 0;

	// Set temp rect's width and height to be same as sdl2CaptionTexture.
	SDL_QueryTexture(sdl2CaptionTexture, NULL, NULL, &tempRect.w, &tempRect.h);
	// Set rect's height to be the smaller value between logoPartialHeight and texture's height.
	tempRect.h = MIN(logoPartialHeight, tempRect.h);
	// Draw rect of sdl2CaptionTexture.
	blitRect(sdl2CaptionTexture, &tempRect, (SCREEN_WIDTH / 2) - (tempRect.w / 2), 100);
	// Set temp rect's width and height to be same as shooterCaptionTexture.
	SDL_QueryTexture(shooterCaptionTexture, NULL, NULL, &tempRect.w, &tempRect.h);
	// Set rect's height to be the smaller value between logoPartialHeight and texture's height.
	tempRect.h = MIN(logoPartialHeight, tempRect.h);
	// Draw rect of shooterCaptionTexture.
	blitRect(shooterCaptionTexture, &tempRect, (SCREEN_WIDTH / 2) - (tempRect.w / 2), 250);
}