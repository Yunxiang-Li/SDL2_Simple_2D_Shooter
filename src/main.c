#include "main.h"

int main(int argc, char* argv[])
{
	// Reset app, player and bullet.
	memset(&app, 0, sizeof(AppStruct));
	memset(&player, 0, sizeof(EntityStruct));
	memset(&bullet, 0, sizeof(EntityStruct));

	// Initialize the SDL with a windows and a renderer.
	initSDL();

	// Set player's position and texture.
	player.x = 100;
	player.y = 100;
	player.texture = loadTexture("Resources/images/player.png");

	// Sed bullet's texture.
	bullet.texture = loadTexture("Resources/images/playerBullet.png");

	// Registers cleanup function to be called on normal program termination.
	atexit(cleanup);

	while (1)
	{
		// Prepare the background part of the game scene inside backbuffer.
		prepareScene();

		// Process input.
		doInput();

		
		//player.x += player.dx;
		//player.y += player.dy;

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

		// Define player's bullet if user fire the bullet(press X key) and current bullet health is not set up yet.
		if (app.fire && bullet.health == 0)
		{
			// Set bullet to be in front of the player and centrally.
			bullet.x = player.x + PLAYER_WIDTH - 20;
			bullet.y = player.y + PLAYER_HEIGHT / 2 - BULLET_HEIGHT / 2;
			// Set bullet's delta movement(20 to the right).
			bullet.dx = 20;
			bullet.dy = 0;
			bullet.health = 1;
		}

		// Update bullet's current position.
		bullet.x += bullet.dx;
		bullet.y += bullet.dy;

		// If bullet is outside the screen, then reset its health.
		if (bullet.x > SCREEN_WIDTH)
			bullet.health = 0;

		// Render player inside backbuffer.
		blit(player.texture, player.x, player.y);

		// Render bullet inside backbuffer if bullet's health is not zero.
		if (bullet.health != 0)
			blit(bullet.texture, bullet.x, bullet.y);

		// Display the whole game scene(from backbuffer).
		presentScene();

		// Wait for 16 miliseconds(FPS will be 62).
		SDL_Delay(16);
	}

	return 0;
}