#include "init.h"

/**
 * @brief Initialize the game.
 *
 */
void initSDL()
{
	// Hide mouse cursor.

	SDL_ShowCursor(0);
	// Set SDL render flags to use hardware acceleration and synchronize with the refresh rate.
	int rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

	// No special flag.
	int windowFlags = 0;

	// Try to initialize SDL video subsystem.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL VIDEO: %s\n", SDL_GetError());
		exit(1);
	}

	// Try to initialize the mixer API. I use 44100 (CD quality), default format, 2 channels (stereo) and 1024 for chunksize.
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		printf("Couldn't initialize SDL Mixer\n");
		exit(1);
	}

	// Set the number of channels being mixed.
	Mix_AllocateChannels(MAX_SOUND_CHANNEL_NUM);

	// Set up game's window with fullcreen(1280 * 720). 
	app.window = SDL_CreateWindow("2D Shooter Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

	// Check if game windows is created successfully.
	if (!app.window)
	{
		printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}

	// Allow linear filtering. (Do I really need this?)
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	// Create the first 2D rendering context which supports the flags mentioned above.
	app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

	// Check if the renderer is created successfully.
	if (!app.renderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	// load support for the JPG and PNG image formats
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initRes = IMG_Init(flags);
	if ((initRes & flags) != flags) {
		// handle error
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
	}
}

/**
 * @brief Clean up the game and quit.
 *
 */
void cleanup()
{
	SDL_DestroyRenderer(app.renderer);

	SDL_DestroyWindow(app.window);

	SDL_Quit();
}

/**
 * @brief Initialize all game preparations including background, starfield, sounds, fonts, highscore table and play the main music.
*/
void initGame()
{
	initBackground();

	initStarfield();

	initSounds();

	initFonts();

	initHighscoreTable();

	// Load and play the main music.
	loadMusic("Resources/music/Mercury.ogg");
	playMusic(1);
}