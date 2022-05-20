#include "main.h"

// Forward declaration.
static void capFrameRate(long*);

// Main game loop.
int main(int argc, char* argv[])
{
	// Declare each frame's start time.
	long frameStartTime;

	// Reset app.
	memset(&app, 0, sizeof(AppStruct));

	// Initialize the SDL with a windows and a renderer.
	initSDL();

	initSounds();

	// Set up all prepare work for player and bullets.
	initStage();

	// Registers cleanup function to be called on normal program termination.
	atexit(cleanup);

	// Start the main game loop.
	while (1)
	{
		// Initialize each frame start time.
		frameStartTime = SDL_GetTicks();

		// Clear the background part of the game scene inside backbuffer.
		prepareScene();

		// Process input.
		doInput();

		// Perform the game logic and draw the scene inside backbuffer.
		app.delegate.logic();
		app.delegate.draw();

		// Display the whole game scene(from backbuffer).
		presentScene();

		// Try to set the game to be nearly const 60 fps.
		capFrameRate(&frameStartTime);
	}

	return 0;
}

static void capFrameRate(long* frameStartTime)
{
	// Set this frame's wait time to each frame's supposed time(16.667 milliseconds).
	long expectedFrameTime = MILISECOND_PER_FRAME;

	// Store the current frame's time.
	long actualFrameTime = SDL_GetTicks() - *frameStartTime;

	// Check if each frame is processed faster then expected.
	if (expectedFrameTime >= actualFrameTime)
	{
		// Subtract actual frame time from expected frame time.
		expectedFrameTime -= actualFrameTime;
		// Wait for remaining time.
		SDL_Delay(expectedFrameTime);
	}

	// Set up next frame's start time.
	*frameStartTime = SDL_GetTicks();
}