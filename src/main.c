#include "main.h"

int main(int argc, char* argv[])
{
	// Reset app and player.
	memset(&app, 0, sizeof(AppStruct));
	memset(&player, 0, sizeof(PlayerStruct));

	// Initialize the SDL with a windows and a renderer.
	initSDL();

	// Set player's position and texture.
	player.x = 100;
	player.y = 100;
	player.texture = loadTexture("Resources/images/player.png");

	// Registers cleanup function to be called on normal program termination.
	atexit(cleanup);

	while (1)
	{
		prepareScene();

		doInput();

		if (app.up)
			player.y -= 4;
		if (app.down)
			player.y += 4;
		if (app.left)
			player.x -= 4;
		if (app.right)
			player.x += 4;


		blit(player.texture, player.x, player.y);

		presentScene();

		// Wait for 16 miliseconds(FPS will be 62).
		SDL_Delay(16);
	}

	return 0;
}