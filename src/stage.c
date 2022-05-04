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
static void resetStage(void);
static void doEnemiesAI(void);
static void clipPlayer(void);
static void fireEnemyBullet(EntityStruct*);

// Declare player by pointer.
static EntityStruct* player;

// Declare player texture and player bullet texture.
static SDL_Texture* playerTexture;
static SDL_Texture* playerBulletTexture;

// Declare enemy texture, enemy bullet texture and spawn timer.
static SDL_Texture* enemyTexture;
static SDL_Texture* enemyBulletTexture;
static int enemySpawnTimer;

// Declare the stage reset timer(decrement after player is dead).
static int stageResetTimer;

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

	// Set player and player bullet's texture.
	playerTexture = loadTexture("Resources/images/player.png");
	playerBulletTexture = loadTexture("Resources/images/playerBullet.png");

	// Set enemy's texture, enemy bullet's texture.
	enemyTexture = loadTexture("Resources/images/enemy.png");
	enemyBulletTexture = loadTexture("Resources/images/enemyBullet.png");

	// Reset the whole game stage.
	resetStage();
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

	// Set player's health, original position, texture, width, height and side.
	player->health = 1;
	player->x = PLAYER_ORIGIN_X;
	player->y = PLAYER_ORIGIN_Y;
	player->texture = playerTexture;
	SDL_QueryTexture(player->texture, NULL, NULL, &player->width, &player->height);
	player->side = PLAYER_SIDE;
}

/**
 * @brief A bound callback to handle all in game updates.
*/
static void logic()
{
	// Update player.
	doPlayer();

	// Update enemies' simple AI.
	doEnemiesAI();

	// Update all shooters' positions.
	doShooters();

	// Update all bullets.
	doBullets();

	// Spawn enemies.
	spawnEnemies();

	// Check player's border.
	clipPlayer();

	// If player is dead and stage timer is less than zero.
	if (player == NULL && --stageResetTimer <= 0)
	{
		resetStage();
	}
}

/**
 * @brief Update the player's states if player is still alive.
*/
static void doPlayer()
{
	if (player != NULL)
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
	}
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
	bullet->texture = playerBulletTexture;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &(bullet->width), &(bullet->height));
	bullet->x = player->x + player->width;
	bullet->y = player->y + player->height / 2 - bullet->height / 2;
	bullet->dx = PLAYER_BULLET_SPEED;
	bullet->health = 1;

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

		// If current bullet is out of the screen or collides with our side shooter, then set its health to zero and delete it.
		if (curr->x > SCREEN_WIDTH || curr->y > SCREEN_HEIGHT || curr->x < -(curr->width) || curr->y < -(curr->height) ||
			isBulletHitShooter(curr))
		{
			curr->health = 0;
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

		// If current enemy shooter is too left(off screen)
		if (/*curr != player && (*/curr->x < -curr->width)
		{
			// Set current enemy shooter's health to zero.
			curr->health == 0;
		}

		// Check if the current shooter's health is 0.
		if (curr->health == 0)
		{	
			// If its player, then destroy the player.
			if (curr == player)
			{
				player = NULL;
			}
			// Handle the tail special case.
			if (curr == stage.shooterTailPtr)
			{
				stage.shooterTailPtr = prev;
			}
			//  Delete current shooter.
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

		// Set each enemy's texture, width and height.
		enemy->texture = enemyTexture;
		SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->width, &enemy->height);
		// Set each enemy's start position.
		enemy->x = SCREEN_WIDTH - enemy->width;
		// Prevent the enemies from leaving the top and bottom of the screen.
		enemy->y = rand() % (SCREEN_HEIGHT - enemy->height);

		// Set each enemy's delta movement on X axis(between -2 and -5).
		enemy->dx = -(2 + (rand() % 4));

		// Set next enemy's spawn time to a integer from 30 to 89.
		enemySpawnTimer = FPS/2 + (rand() % FPS);
		// Set enemy's side.
		enemy->side = ENEMY_SIDE;
		// Set enemy's health.
		enemy->health = 1;
		// Set enemy's bullet cooldown(from 1 second to 2 seconds under 60 fps).
		enemy->bulletCooldown = FPS * (1 + (rand() % 2));
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
		if (currShooter->side != bullet->side && collision(&bulletRect, &currShooterRect))
		{
			// Set bullet and currShooter's health to zero(destroy bullet and currShooter).
			bullet->health = 0;
			currShooter->health = 0;
			return 1;
		}
	}
	return 0;
}

/**
 * @brief Reset the whole game stage.
*/
static void resetStage()
{
	// Declare two pointer to help us free two linked lists.
	EntityStruct* currShooterPtr;
	EntityStruct* currBulletPtr;

	// Delete all shooters in the shooter linked list.
	while (stage.shooterHead.next)
	{
		currShooterPtr = stage.shooterHead.next;
		stage.shooterHead.next = currShooterPtr->next;
		free(currShooterPtr);
	}

	// Delete all bullets in the bullet linked list.
	while (stage.bulletHead.next)
	{
		currBulletPtr = stage.bulletHead.next;
		stage.bulletHead.next = currBulletPtr->next;
		free(currBulletPtr);
	}

	// Reset the stage struct and two linked lists.
	memset(&stage, 0, sizeof(StageStruct));
	stage.shooterTailPtr = &stage.shooterHead;
	stage.bulletTailPtr = &stage.bulletHead;

	// Re-initialize the player.
	initPlayer();

	// Reset enemy spawn timer.
	enemySpawnTimer = 0;
	// Reset stage reset timer.
	stageResetTimer = FPS * 2;
}

/**
 * @brief Update enemies AI.
*/
static void doEnemiesAI()
{
	// Loop throught the shooter linked list.
	for (EntityStruct* currEnemyPtr = stage.shooterHead.next; currEnemyPtr != NULL; currEnemyPtr = currEnemyPtr->next)
	{
		// Check if current shooter is not player, player is still alive and current enemy is able to fire next bullet.
		if (currEnemyPtr != player && player != NULL && --currEnemyPtr->bulletCooldown <= 0)
		{
			// Fire the bullet
			fireEnemyBullet(currEnemyPtr);
		}
	}
}

/**
 * @brief Let input enemy fire a bullet towards player's position.
 * @param enemyPtr A pointer of EntityStruct indicates the input enemy object.
*/
static void fireEnemyBullet(EntityStruct* enemyPtr)
{
	// Initialize a bullet object and insert it into the bullet linked list.
	EntityStruct* bullet = malloc(sizeof(EntityStruct));
	memset(bullet, 0, sizeof(EntityStruct));
	stage.bulletTailPtr->next = bullet;
	stage.bulletTailPtr = bullet;

	// Set enemy bullet's position, health, texture, side, width and height.
	bullet->texture = enemyBulletTexture;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->width, &bullet->height);
	bullet->x = enemyPtr->x - (bullet->width / 2);
	bullet->y = enemyPtr->y + (enemyPtr->height / 2) - (bullet->height / 2);
	bullet->health = 1;
	bullet->side = enemyPtr->side;


	// Normalize enemy bullet's dx and dy towards player's direction.
	calcSlope(player->x + (player->width / 2), player->y + (player->height / 2), bullet->x, bullet->y, &bullet->dx, &bullet->dy);

	// Multiply normalized enemy bullet direction by enemy bullet speed.
	bullet->dx *= ENEMY_BULLET_SPEED;
	bullet->dy *= ENEMY_BULLET_SPEED;

	// Set enemy's bullet's side and cooldown(2 seconds under 60 fps).
	bullet->side = ENEMY_SIDE;
	enemyPtr->bulletCooldown = (rand() % FPS * 2);
}

/**
 * @brief Player's border check.
*/
static void clipPlayer()
{
	if (player != NULL)
	{
		if (player->x < 0)
		{
			player->x = 0;
		}

		if (player->y < 0)
		{
			player->y = 0;
		}

		if (player->x > SCREEN_WIDTH - player->width)
		{
			player->x = SCREEN_WIDTH - player->width;
		}

		if (player->y > SCREEN_HEIGHT - player->height)
		{
			player->y = SCREEN_HEIGHT - player->height;
		}
	}
}