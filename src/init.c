#include "init.h"

/**
 * @brief Initialize the SDL with a windows and a renderer(for now).
 *
 */
void initSDL()
{
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
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
		// handle error
	}
}

/**
 * @brief Clean up the appStruct and quit.
 *
 */
void cleanup(void)
{
	SDL_DestroyRenderer(app.renderer);

	SDL_DestroyWindow(app.window);

	SDL_Quit();
}