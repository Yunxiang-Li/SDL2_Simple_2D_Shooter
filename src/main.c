#include "main.h"

int main(int argc, char* argv[])
{
	// Set appStruct's 2 pointers to be nullptr.
	memset(&appStruct, 0, sizeof(AppStruct));

	// Initialize the SDL with a windows and a renderer.
	initSDL();

	// Registers cleanup function to be called on normal program termination.
	atexit(cleanup);

	while (1)
	{
		prepareScene();

		doInput();

		presentScene();

		// Wait for 16 miliseconds(FPS will be 62).
		SDL_Delay(16);
	}

	return 0;
}