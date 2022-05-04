#include <stage.h>

// Forward declarations.
static void logic(void);
static void draw(void);
static void initPlayer(void);
static void doPlayer(void);
static void doBullets(void);
static void fireBullet(void);
static void drawBullets(void);
static void doShooters(void);
static void spawnEnemies(void);
static void drawShooters(void);
static int isBulletHitShooter(EntityStruct*);

// Declare player by pointer.
static EntityStruct* player;

// Declare bullet texture
static SDL_Texture* bulletTexture;

// Declare enemy texture and spawn timer.
static SDL_Texture* enemyTexture;
static int enemySpawnTimer;

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

	// Setup two linked lists for shooters and bullets.
	stage.shooterTailPtr = &stage.shooterHead;
	stage.bulletTailPtr = &stage.bulletHead;

	// Initialize the player.
	initPlayer();

	// Set bullet's texture.
	bulletTexture = loadTexture("Resources/images/playerBullet.png");

	// Set enemy's texture and spawn timer.
	enemyTexture = loadTexture("Resources/images/enemy.png");
	enemySpawnTimer = 0;
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

	// Add player to shooter linked list.
	stage.shooterTailPtr->next = player;
	stage.shooterTailPtr = player;

	// Set player's original position, texture, width, height and side.
	player->x = PLAYER_ORIGIN_X;
	player->y = PLAYER_ORIGIN_Y;
	player->texture = loadTexture("Resources/images/player.png");
	SDL_QueryTexture(player->texture, NULL, NULL, &player->width, &player->height);
	player->side = PLAYER_SIDE;
}

/**
 * @brief A bound callback to set up player's delta movement, update all planes' positions(including player),
 update player bullets' positions and then spawn all enemies.
*/
static void logic()
{
	doPlayer();

	doShooters();

	doBullets();

	spawnEnemies();
}

/**
 * @brief Update the player's states.
*/
static void doPlayer()
{
	// Set player's delta movement to zero.
	player->dx = 0;
	player->dy = 0;

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

	// Reload attribute controls how fast player can fire bullets.
	if (player->bulletCooldown > 0)
	{
		(player->bulletCooldown--);
	}

	// Check if player press fire key(X key) and reload(actually cooldown) is reduced to zero, then fire a bullet.
	if (app.keyboard[SDL_SCANCODE_X] && player->bulletCooldown == 0)
	{
		fireBullet();
	}

	//// Here.
	//// Update player's current position.
	//player->x += player->dx;
	//player->y += player->dy;

	//// Border check
	//if (player->y > SCREEN_HEIGHT - PLAYER_HEIGHT)
	//	player->y = SCREEN_HEIGHT - PLAYER_HEIGHT;
	//if (player->y < 0)
	//	player->y = 0;
	//if (player->x < 0)
	//	player->x = 0;
	//if (player->x > SCREEN_WIDTH - PLAYER_WIDTH)
	//	player->x = SCREEN_WIDTH - PLAYER_WIDTH;
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
	stage.bulletTailPtr->next = bullet;
	stage.bulletTailPtr = bullet;

	// Set up bullet's position, width, height, delta movement, health and texture.
	bullet->x = player->x + PLAYER_WIDTH;
	bullet->y = player->y + PLAYER_HEIGHT / 2 - PLAYER_BULLET_HEIGHT / 2;
	bullet->dx = PLAYER_BULLET_SPEED;
	bullet->health = 1;
	bullet->texture = bulletTexture;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &(bullet->width), &(bullet->height));

	// Set the player reload(0.133333 seconds for next bullet in 60fps situation).
	player->bulletCooldown = PLAYER_BULLET_COOLDOWN;
	// Set player bullets' side.
	bullet->side = PLAYER_SIDE;
}

/**
 * @brief Update the bullet's states.
*/
static void doBullets(void)
{
	// Initialize two EntityStruct pointers as dummy head and start node of the bullet linked list.
	EntityStruct* prev = &stage.bulletHead;
	EntityStruct* curr;

	// Loop the entire bullet linked list.
	for (curr = stage.bulletHead.next; curr != NULL; curr = curr->next)
	{
		// Update each bullet position.
		curr->x += curr->dx;
		curr->y += curr->dy;

		// If current bullet is out the screen then delete it.
		if (curr->x > SCREEN_WIDTH || isBulletHitShooter(curr))
		{
			 //Check for tail special condition.
			if (curr == stage.bulletTailPtr)
			{
				stage.bulletTailPtr = prev;
			}
			prev->next = curr->next;
			free(curr);
			curr = prev;
		}
		prev = curr;
	}
}

/**
 * @brief Draw all shooters and all bullets.
*/
static void draw()
{
	drawShooters();

	drawBullets();
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

/**
 * @brief update all shooters' positions(including player)
*/
static void doShooters()
{
	// Initialize two EntityStruct pointers as two start nodes of the shooter linked list.
	EntityStruct* curr, * prev;

	// Set prev as the dummy head of the shooter linked list.
	prev = &stage.shooterHead;

	// Loop entire shooter linked list.
	for (curr = stage.shooterHead.next; curr != NULL; curr = curr->next)
	{
		// Update each shooter's position.
		curr->x += curr->dx;
		curr->y += curr->dy;

		// Player border check.
		if (curr == player)
		{
			if (curr->y > SCREEN_HEIGHT - PLAYER_HEIGHT)
				curr->y = SCREEN_HEIGHT - PLAYER_HEIGHT;
			if (curr->y < 0)
				curr->y = 0;
			if (curr->x < 0)
				curr->x = 0;
			if (curr->x > SCREEN_WIDTH - PLAYER_WIDTH)
				curr->x = SCREEN_WIDTH - PLAYER_WIDTH;
		}

		// Check if the enemy shooter is too left(fully offscreen) or is detroyed(health equals 0).
		if (curr != player && (curr->x < -curr->width || curr->health == 0))
		{	
			if (curr == stage.shooterTailPtr)
			{
				stage.shooterTailPtr = prev;
			}
			// If so, delete it.
			prev->next = curr->next;
			free(curr);
			curr = prev;
		}
		prev = curr;
	}
}

/**
 * @brief Spawn an enemy object at the right side of the screen.
*/
static void spawnEnemies()
{
	// Declare the next enemy to be spawned.
	EntityStruct* enemy;

	// Check if we are able to spawn next enemy.
	if (enemySpawnTimer-- == 0)
	{
		// Create and initialize the new enemy object.
		enemy = malloc(sizeof(EntityStruct));
		memset(enemy, 0, sizeof(EntityStruct));

		// Add new enemy to the tail of the shooter linked list.
		stage.shooterTailPtr->next = enemy;
		stage.shooterTailPtr = enemy;

		// Set each enemy's start position, texture and width and height.
		enemy->x = SCREEN_WIDTH - ENEMY_WIDTH;
		// Prevent the enemies from leaving the top and bottom of the screen.
		enemy->y = rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT);
		enemy->texture = enemyTexture;
		SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->width, &enemy->height);

		// Set each enemy's delta movement on X axis(between -2 and -5).
		enemy->dx = -(2 + (rand() % 4));
		//enemy->dy = 1 -(rand() % 3);

		// Set next enemy's spawn time to a integer from 30 to 89.
		enemySpawnTimer = 30 + (rand() % 60);
		// Set enemy's side.
		enemy->side = ENEMY_SIDE;
		// Set enemy's health.
		enemy->health = 1;
	}
}

/**
 * @brief  Draw all shooters include player.
*/
static void drawShooters()
{
	for (EntityStruct* shooter = stage.shooterHead.next; shooter != NULL; shooter = shooter->next)
	{
		blit(shooter->texture, shooter->x, shooter->y);
	}
}

/**
 * @brief Check if input bullet object collides with any of the other side shooters.
 * @param bullet A pointer of EntityStruct object indicates the input bullet object.
 * @return 1 for collide and 0 for non-collide.
*/
static int isBulletHitShooter(EntityStruct* bullet)
{
	// Loop throught the shooter linked list.
	for (EntityStruct* currShooter = stage.shooterHead.next; currShooter != NULL; currShooter = currShooter->next)
	{
		// Define current bullet and each shooter's SDL_Rect object.
		SDL_Rect bulletRect = { bullet->x, bullet->y, bullet->width, bullet->height };
		SDL_Rect currShooterRect = { currShooter->x, currShooter->y, currShooter->width, currShooter->height };

		// Check if bullet and shooter have different sides and two SDL_Rect objects do overlap.
		if (currShooter->side != bullet->side && collision(&bulletRect, currShooterRect))
		{
			// Set bullet and currShooter's health to zero(destroy bullet and currShooter).
			bullet->health = 0;
			currShooter->health = 0;
			return 1;
		}
	}
	return 0;
}