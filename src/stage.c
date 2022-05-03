#include <stage.h>

// Forward declarations.
static void logic(void);
static void draw(void);
static void initPlayer(void);
static void doPlayer(void);
static void doBullets(void);
static void fireBullet(void);
static void doBullets(void);
static void drawPlayer(void);
static void drawBullets(void);

// Declare player by pointer.
static EntityStruct* player;

// Declare bullet texture
static SDL_Texture* bulletTexture;

/**
 * @brief Set up all prepare work.
*/
void initStage()
{
	// Bind logic and draw functions.
	app.delegate.logic = logic;
	app.delegate.draw = draw;
	// Reset the stage struct.
	memset(&stage, 0, sizeof(StageStruct));

	// Setup two linked lists for fighters and bullets.
	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;

	initPlayer();

	// Set bullet's texture.
	bulletTexture = loadTexture("Resources/images/playerBullet.png");
}

/**
 * @brief Set up the player.
*/
static void initPlayer()
{	
	// Dynamically assign the memory.
	player = malloc(sizeof(EntityStruct));
	// Set all player attributes to zero.
	memset(player, 0, sizeof(EntityStruct));

	// Add player to fighter linked list.
	stage.fighterTail->next = player;
	stage.fighterTail = player;

	// Set player's original position, texture, width and height.
	player->x = 100;
	player->y = 100;
	player->texture = loadTexture("Resources/images/player.png");
	SDL_QueryTexture(player->texture, NULL, NULL, &player->width, &player->height);
}

/**
 * @brief A bound callback to process player and bullets' updates.
*/
static void logic()
{
	doPlayer();

	doBullets();
}

/**
 * @brief Update the player's states.
*/
static void doPlayer()
{
	// Set player's delta movement to zero.
	player->dx = 0;
	player->dy = 0;

	// Reload attribute controls how fast player can fire bullets.
	if (player->reload > 0)
	{
		--(player->reload);
	}

	// Update player's delta movement.
	if (app.keyboard[SDL_SCANCODE_UP])
	{
		player->dy = -PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_DOWN])
	{
		player->dy = PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_LEFT])
	{
		player->dx = -PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_RIGHT])
	{
		player->dx = PLAYER_SPEED;
	}

	// Check if player press fire key(X key) and reload(actually cooldown) is reduced to zero, then fire a bullet.
	if (app.keyboard[SDL_SCANCODE_X] && player->reload == 0)
	{
		fireBullet();
	}

	// Update player's current position.
	player->x += player->dx;
	player->y += player->dy;

	// Border check
	if (player->y > SCREEN_HEIGHT - PLAYER_HEIGHT)
		player->y = SCREEN_HEIGHT - PLAYER_HEIGHT;
	if (player->y < 0)
		player->y = 0;
	if (player->x < 0)
		player->x = 0;
	if (player->x > SCREEN_WIDTH - PLAYER_WIDTH)
		player->x = SCREEN_WIDTH - PLAYER_WIDTH;
}

/**
 * @brief Process the behavior of firing a bullet.
*/
static void fireBullet()
{
	// Initialize the bullet.
	EntityStruct* bullet;
	bullet = malloc(sizeof(EntityStruct));
	memset(bullet, 0, sizeof(EntityStruct));

	// Add the bullet into the bullet linked list.
	stage.bulletTail->next = bullet;
	stage.bulletTail = bullet;

	// Set up bullet's position, width, height, delta movement, health and texture.
	bullet->x = player->x + PLAYER_WIDTH - 20;
	bullet->y = player->y + PLAYER_HEIGHT / 2 - BULLET_HEIGHT / 2;
	bullet->dx = PLAYER_BULLET_SPEED;
	bullet->health = 1;
	bullet->texture = bulletTexture;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &(bullet->width), &(bullet->height));

	// Set the player reload(0.133333 seconds for next bullet in 60fps situation).
	player->reload = 8;
}

/**
 * @brief Update the bullet's states.
*/
static void doBullets(void)
{
	// Initialize two EntityStruct pointers as two start nodes of the bullet linked list.
	EntityStruct* curr;
	EntityStruct* prev = &stage.bulletHead;

	// Loop the entire bullet linked list.
	for (curr = stage.bulletHead.next; curr != NULL; curr = curr->next)
	{
		// Update each bullet position.
		curr->x += curr->dx;
		curr->y += curr->dy;

		// If current bullet is out the screen then delete it.
		if (curr->x > SCREEN_WIDTH)
		{
			if (curr == stage.bulletTail)
			{
				stage.bulletTail = prev;
			}
			prev->next = curr->next;
			free(curr);
			curr = prev;
		}
		prev = curr;
	}
}

/**
 * @brief Draw player and all bullets.
*/
static void draw()
{
	drawPlayer();

	drawBullets();
}

/**
 * @brief Draw the player at specified position.
*/
static void drawPlayer()
{
	blit(player->texture, (int)player->x, (int)player->y);
}

/**
 * @brief Draw all bullets at specified positions.
*/
static void drawBullets()
{

	for (EntityStruct* bullet = stage.bulletHead.next; bullet != NULL; bullet = bullet->next)
	{
		blit(bullet->texture, (int)bullet->x, (int)bullet->y);
	}
}