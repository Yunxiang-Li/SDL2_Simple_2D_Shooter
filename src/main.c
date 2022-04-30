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
		// Prepare the background part of the game scene inside backbuffer.
		prepareScene();

		// Process input.
		doInput();

		// Update player's current position.
		if (app.up)
		{
			player.y -= 5;
			if (player.y < 0)
				player.y = 0;
		}
	
		if (app.down)
		{
			player.y += 5;
			if (player.y > SCREEN_HEIGHT - PLAYER_HEIGHT)
				player.y = SCREEN_HEIGHT - PLAYER_HEIGHT;
		}
			
		if (app.left)
		{
			player.x -= 5;
			if (player.x < 0)
				player.x = 0;
		}
			
		if (app.right)
		{
			player.x += 5;
			if (player.x > SCREEN_WIDTH - PLAYER_WIDTH)
				player.x = SCREEN_WIDTH - PLAYER_WIDTH;
		}

		// Render player inside backbuffer.
		blit(player.texture, player.x, player.y);

		// Display the whole game scene(from backbuffer).
		presentScene();

		// Wait for 16 miliseconds(FPS will be 62).
		SDL_Delay(16);
	}

	return 0;
}