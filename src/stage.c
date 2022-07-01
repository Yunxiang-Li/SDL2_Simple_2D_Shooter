#include <stage.h>

// Forward declarations.
static void logic(void);
static void draw(void);
static void initPlayer(void);
static void updatePlayer(void);
static void updateBullets(void);
static void fireBullet(void);
static void drawBullets(void);
static void updateShooters(void);
static void spawnEnemies(void);
static void drawShooters(void);
static int isBulletHitShooter(EntityStruct*);
static void resetStage(void);
static void updateEnemiesAI(void);
static void clipPlayer(void);
static void fireEnemyBullet(EntityStruct*);
static void updateExplosions(void);
static void updateDebris(void);
static void addExplosions(int, int, int);
static void addDebris(EntityStruct*);
static void drawDebris(void);
static void drawExplosions(void);
static void drawHud(void);
static void updatePointPods(void);
static void addPointPod(int x, int y);
static void drawPointPods(void);

// Declare player by pointer.
static EntityStruct* player;

// Declare textures for player, player's bullet, game background, explosion and pointPod.
static SDL_Texture* playerTexture;
static SDL_Texture* playerBulletTexture;
static SDL_Texture* backgroundTexture;
static SDL_Texture* explosionTexture;
static SDL_Texture* pointPodTexture;

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

	// Setup four linked lists for shooters, bullets, explosions, debris and pointPod.
	stage.shooterTailPtr = &stage.shooterHead;
	stage.bulletTailPtr = &stage.bulletHead;
	stage.explosionTailPtr = &stage.explosionHead;
	stage.debrisTailPtr = &stage.debrisHead;
	stage.pointPodTailPtr = &stage.bulletHead;

	// Set player and player bullet's texture.
	playerTexture = loadTexture("Resources/images/player.png");
	playerBulletTexture = loadTexture("Resources/images/playerBullet.png");

	// Set enemy's texture, enemy bullet's texture.
	enemyTexture = loadTexture("Resources/images/enemy.png");
	enemyBulletTexture = loadTexture("Resources/images/enemyBullet.png");
	
	// Set game background texture, explosion texture.
	//backgroundTexture = loadTexture("Resources/images/background.png");
	explosionTexture = loadTexture("Resources/images/explosion.png");

	// Set point pod's texture.
	pointPodTexture = loadTexture("Resources/images/pointPod.png");

	// Reset the keyboard state to zero.
	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);
	// Reset the whole game stage.
	resetStage();
	// Reset score to be 0.
	stage.score = 0;
	// Re-initialize the player.
	initPlayer();
	// Reset enemy spawn timer.
	enemySpawnTimer = 0;
	// Reset stage reset timer to 3 seconds under 60 FPS case.
	stageResetTimer = FPS * 3;
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
	// Border check of background's x position.
	updateBackground();

	// Update all stars.
	updateStarfield();

	// Update player.
	updatePlayer();

	// Update enemies' simple AI.
	updateEnemiesAI();

	// Update all shooters' positions.
	updateShooters();

	// Update explosions and debris.
	updateExplosions();
	updateDebris();

	// Update all bullets.
	updateBullets();

	// Spawn enemies.
	spawnEnemies();

	// Check player's border.
	clipPlayer();

	// Update all point pods objects.
	updatePointPods();

	// If player is dead and stage timer is less than zero, update the highscore table with latest score and re-initialize highscore feature.
	if (player == NULL && --stageResetTimer <= 0)
	{
		addHighscore(stage.score);
		initHighscores();
	}
}

/**
 * @brief Update the player's states if player is still alive.
*/
static void updatePlayer()
{
	if (player != NULL)
	{
		// Set player's delta movement to zero.
		player->dx = 0;
		player->dy = 0;

		// Update player's delta movement.
		if (app.keyboard[SDL_SCANCODE_UP])
		{
			player->dy -= PLAYER_SPEED;
		}
		if (app.keyboard[SDL_SCANCODE_DOWN])
		{
			player->dy += PLAYER_SPEED;
		}
		if (app.keyboard[SDL_SCANCODE_LEFT])
		{
			player->dx -= PLAYER_SPEED;
		}
		if (app.keyboard[SDL_SCANCODE_RIGHT])
		{
			player->dx += PLAYER_SPEED;
		}

		// Reload attribute controls how fast player can fire bullets.
		if (player->bulletCooldown > 0)
		{
			(--player->bulletCooldown);
		}

		// Check if player press fire key(X key) and reload(actually cooldown) is reduced to zero, then fire a bullet and play the sound.
		if (app.keyboard[SDL_SCANCODE_X] && player->bulletCooldown == 0)
		{
			fireBullet();
			playSound(SND_PLAYER_FIRE, CH_PLAYER);
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
static void updateBullets(void)
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
 * @brief Draw the background, star field, point pods, shooters, debris, explosions, bullets and Hud.
*/
static void draw()
{
	drawBackground();

	drawStarfield();

	drawPointPods();

	drawShooters();

	drawDebris();

	drawExplosions();

	drawBullets();

	drawHud();
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
static void updateShooters()
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
			curr->health = 0;
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
		if (currShooter->side != bullet->side && hasCollision(&bulletRect, &currShooterRect))
		{
			// Set bullet and currShooter's health to zero(destroy bullet and currShooter).
			bullet->health = 0;
			currShooter->health = 0;

			// Add explosion and 4 debris for current shooter.
			addExplosions(currShooter->x, currShooter->y, 32);
			addDebris(currShooter);

			// If player is shot, play player death sound.
			if (currShooter == player)
				playSound(SND_PLAYER_DIE, CH_PLAYER);
			// If enemy is shot.
			else
			{
				// Add one point pod object at the center of the enemy object.
				addPointPod(currShooter->x + currShooter->width / 2, currShooter->y + currShooter->height / 2);
				// Play enemy death sound.
				playSound(SND_ENEMY_DIE, CH_ANY);
			}
			return 1;
		}
	}
	return 0;
}

/**
 * @brief Reset all five linked lists.
*/
static void resetStage()
{
	// Declare five pointers to help us free four linked lists.
	EntityStruct* currShooterPtr;
	EntityStruct* currBulletPtr;
	ExplosionStruct* currExplosionPtr;
	DebrisStruct* currDebrisPtr;
	EntityStruct* currPointPodPtr;

	// Delete all shooters in the shooter linked list.
	while (stage.shooterHead.next != NULL)
	{
		currShooterPtr = stage.shooterHead.next;
		stage.shooterHead.next = currShooterPtr->next;
		free(currShooterPtr);
	}

	// Delete all bullets in the bullet linked list.
	while (stage.bulletHead.next != NULL)
	{
		currBulletPtr = stage.bulletHead.next;
		stage.bulletHead.next = currBulletPtr->next;
		free(currBulletPtr);
	}

	// Delete all explosions in the explosion linked list.
	while (stage.explosionHead.next != NULL)
	{
		currExplosionPtr = stage.explosionHead.next;
		stage.explosionHead.next = currExplosionPtr->next;
		free(currExplosionPtr);
	}

	// Delete all debris in the debris linked list.
	while (stage.debrisHead.next != NULL)
	{
		currDebrisPtr = stage.debrisHead.next;
		stage.debrisHead.next = currDebrisPtr->next;
		free(currDebrisPtr);
	}

	// Delete all point pods in the point pod linked list.
	while (stage.pointPodHead.next != NULL)
	{
		currPointPodPtr = stage.pointPodHead.next;
		stage.pointPodHead.next = currPointPodPtr->next;
		free(currPointPodPtr);
	}

	// Reset the stage struct and five linked lists.
	memset(&stage, 0, sizeof(StageStruct));
	stage.shooterTailPtr = &stage.shooterHead;
	stage.bulletTailPtr = &stage.bulletHead;
	stage.explosionTailPtr = &stage.explosionHead;
	stage.debrisTailPtr = &stage.debrisHead;
	stage.pointPodTailPtr = &stage.pointPodHead;
}

/**
 * @brief Update enemies AI.
*/
static void updateEnemiesAI()
{
	// Loop throught the shooter linked list.
	for (EntityStruct* currEnemyPtr = stage.shooterHead.next; currEnemyPtr != NULL; currEnemyPtr = currEnemyPtr->next)
	{
		// Check if current shooter is not player, player is still alive and current enemy is able to fire next bullet.
		if (currEnemyPtr != player && player != NULL && --currEnemyPtr->bulletCooldown <= 0)
		{
			// Fire the bullet and play the sound.
			fireEnemyBullet(currEnemyPtr);
			playSound(SND_ENEMY_FIRE, CH_ENEMY_FIRE);
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

/**
 * @brief Update the whole explosion linked list.
*/
static void updateExplosions()
{
	// Iterate through the explosion linked list.
	for (ExplosionStruct* currExplosionPtr = stage.explosionHead.next, *prevExplosionPtr = &stage.explosionHead;
		currExplosionPtr != NULL; currExplosionPtr = currExplosionPtr->next)
	{
		// Update each explosion's position.
		currExplosionPtr->x += currExplosionPtr->dx;
		currExplosionPtr->y += currExplosionPtr->dy;

		// Decrement current explosion's alpha value and check if it is not positive.
		if (--currExplosionPtr->alpha <= 0)
		{
			// Delete current explosion node.

			// Check for tail corner case.
			if (currExplosionPtr == stage.explosionTailPtr)
			{
				stage.explosionTailPtr = prevExplosionPtr;
			}
			prevExplosionPtr->next = currExplosionPtr->next;
			free(currExplosionPtr);
			currExplosionPtr = prevExplosionPtr;
		}

		prevExplosionPtr = currExplosionPtr;
	}
}

/**
 * @brief Update the whole debris linked list.
*/
static void updateDebris()
{
	// Iterate through the whole debris linked list.
	for (DebrisStruct* currDebris = stage.debrisHead.next, *prevDebris = &stage.debrisHead;
		currDebris != NULL; currDebris = currDebris->next)
	{
		// Update current debris's position.
		currDebris->x += currDebris->dx;
		currDebris->y += currDebris->dy;

		// Accelerate current debris by 0.5 on y axis(down).
		currDebris->dy += 0.5;

		// Decrement current debris life and check if its not positive.
		if (--currDebris->life <= 0)
		{
			// Delete current debris node.

			// Check for tail corner case.
			if (currDebris == stage.debrisTailPtr)
			{
				stage.debrisTailPtr = prevDebris;
			}

			prevDebris->next = currDebris->next;
			free(currDebris);
			currDebris = prevDebris;
		}

		prevDebris = currDebris;
	}
}

/**
 * @brief Add certain number of explosions whose origin are at specified position.
 * @param x An integer indicates origin position's x.
 * @param y An integer indicates origin position's y.
 * @param num An integer indicates the number of added explosions.
*/
static void addExplosions(int x, int y, int num)
{
	// Store each dynamically allocated
	ExplosionStruct* currExplosionPtr;

	// Create each new explosion.
	for (int i = 0; i < num; ++i)
	{
		// Allocate memory for each new explosion and initialize each attribute to 0.
		currExplosionPtr = malloc(sizeof(ExplosionStruct));
		memset(currExplosionPtr, 0, sizeof(ExplosionStruct));

		// Add current explosion object into explosion linked list.
		stage.explosionTailPtr->next = currExplosionPtr;
		stage.explosionTailPtr = currExplosionPtr;

		// Randomlize each added explosion's origin position a little bit(from +31 to -31).
		currExplosionPtr->x = x + (rand() % 32) - (rand() % 32);
		currExplosionPtr->y = y + (rand() % 32) - (rand() % 32);

		// Randomlize each added explosion's delta movement a little bit(from +0.9 to -0.9).
		currExplosionPtr->dx = (rand() % 10) - (rand() % 10);
		currExplosionPtr->dy = (rand() % 10) - (rand() % 10);
		currExplosionPtr->dx /= 10;
		currExplosionPtr->dy /= 10;

		// Choose 1 of 4 cases for current explosion's color.
		switch (rand() % 4)
		{
		case 0:
			currExplosionPtr->red = 255;
			break;

		case 1:
			currExplosionPtr->red = 255;
			currExplosionPtr->green = 128;
			break;

		case 2:
			currExplosionPtr->red = 255;
			currExplosionPtr->green = 255;
			break;

		default:
			currExplosionPtr->red = 255;
			currExplosionPtr->green = 255;
			currExplosionPtr->blue = 255;
			break;
		}

		// Randomlize current explosion's alpha(from 0 to 3) which also affect explosion's lifetime.
		currExplosionPtr->alpha = rand() % FPS * 3;
	}
}

/**
 * @brief Generate four debris for input spaceship.
 * @param spaceshipPtr A pointer of EntityStruct indicates the input spaceship.
*/
static void addDebris(EntityStruct* spaceshipPtr)
{
	DebrisStruct* addedDebrisPtr;

	// Divide and store input spaceship's width and height by 2.
	int width = spaceshipPtr->width / 2;
	int height = spaceshipPtr->height / 2;

	// Generate 4 debris at (width/2, height/2), (width, height/2), (width/2, height) and (width, height).
	for (int y = 0; y <= height; y += height)
	{
		for (int x = 0; x <= width; x += width)
		{
			// Dynamically allocate memory for each added debris and initialize all attributes with 0.
			addedDebrisPtr = malloc(sizeof(DebrisStruct));
			memset(addedDebrisPtr, 0, sizeof(DebrisStruct));

			// Insert newly created debris into the debris linked list.
			stage.debrisTailPtr->next = addedDebrisPtr;
			stage.debrisTailPtr = addedDebrisPtr;

			// Set up added debris position(at the center of input spaceship), delta movement, life and texture.
			addedDebrisPtr->x = spaceshipPtr->x + spaceshipPtr->width / 2;
			addedDebrisPtr->y = spaceshipPtr->y + spaceshipPtr->height / 2;
			// Random from -4 to 4.
			addedDebrisPtr->dx = (rand() % 5) - (rand() % 5);
			// Random from -16 to -5.
			addedDebrisPtr->dy = -(5 + (rand() % 12));
			// Two life under 60 fps condition.
			addedDebrisPtr->life = FPS * 2;
			addedDebrisPtr->texture = spaceshipPtr->texture;
			// Set texture's position and size.
			addedDebrisPtr->rect.x = x;
			addedDebrisPtr->rect.y = y;
			addedDebrisPtr->rect.w = width;
			addedDebrisPtr->rect.h = height;
		}
	}
}

/**
 * @brief Draw all debris inside the debris linked list.
*/
static void drawDebris()
{

	for (DebrisStruct* currDebrisPtr = stage.debrisHead.next; currDebrisPtr != NULL; currDebrisPtr = currDebrisPtr->next)
	{
		blitRect(currDebrisPtr->texture, &currDebrisPtr->rect, currDebrisPtr->x, currDebrisPtr->y);
	}
}

/**
 * @brief Draw all explosion inside the explosion linked list.
*/
static void drawExplosions()
{
	// Set render's blend mode as additive blending to draw explosion texture.
	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

	// Color each explosion texture with previous set color(red, green, blue) and alpha, then draw it.
	for (ExplosionStruct* currExplosionPtr = stage.explosionHead.next; currExplosionPtr != NULL; currExplosionPtr = currExplosionPtr->next)
	{
		SDL_SetTextureColorMod(explosionTexture, currExplosionPtr->red, currExplosionPtr->green, currExplosionPtr->blue);
		SDL_SetTextureAlphaMod(explosionTexture, currExplosionPtr->alpha);

		blit(explosionTexture, currExplosionPtr->x, currExplosionPtr->y);
	}

	// Reset render's blend mode to none.
	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}

/**
 * @brief Draw in-game Hud.
*/
static void drawHud()
{
	// Draw player's current score in white on the left side.
	drawText(10, 10, 255, 255, 255, TEXT_LEFT, "SCORE: %03d", stage.score);

	// Draw player's high score on the right side.
	if (stage.score > 0 && stage.score >= highscoreTable.highscore[0].score)
	{
		// If already reaches the high score, then draw in green.
		drawText(960, 10, 0, 255, 0, TEXT_RIGHT, "HIGH SCORE: %03d", stage.score);
	}
	else
	{
		// Draw in white.
		drawText(960, 10, 255, 255, 255, TEXT_RIGHT, "HIGH SCORE: %03d", highscoreTable.highscore[0].score);
	}
}

/**
 * @brief update all point pods from the point pod linked list.
*/
static void updatePointPods()
{
	// Declare two pointers to help traverse the point pod linked list.
	EntityStruct* prevPointPod;
	prevPointPod = &stage.pointPodHead;
	for (EntityStruct* currPointPod = stage.pointPodHead.next; currPointPod != NULL; currPointPod = currPointPod->next)
	{
		// Current point pod's x's border check.
		if (currPointPod->x < 0)
		{
			currPointPod->x = 0;
			currPointPod->dx = -currPointPod->dx;
		}
		if (currPointPod->x + currPointPod->width > SCREEN_WIDTH)
		{
			currPointPod->x = SCREEN_WIDTH - currPointPod->width;
			currPointPod->dx = -currPointPod->dx;
		}
		// Current point pod's y's border check.
		if (currPointPod->y < 0)
		{
			currPointPod->y = 0;
			currPointPod->dy = -currPointPod->dy;
		}
		if (currPointPod->y + currPointPod->height > SCREEN_HEIGHT)
		{
			currPointPod->y = SCREEN_HEIGHT - currPointPod->height;
			currPointPod->dy = -currPointPod->dy;
		}

		// Update current point pod's position.
		currPointPod->x += currPointPod->dx;
		currPointPod->y += currPointPod->dy;

		// Create SDL_Rect object for current point pod.
		SDL_Rect currPointPodRect = { currPointPod->x, currPointPod->y, currPointPod->width, currPointPod->height };

		// Check if player is alivet.
		if (player != NULL)
		{
			// Create SDL_Rect object for the player.
			SDL_Rect playerRect = { player->x, player->y, player->width, player->height };
			//  Check if player collides with current point pod object.
			if (hasCollision(&currPointPodRect, &playerRect))
			{
				// Set current point pod's health to zero.
				currPointPod->health = 0;

				// Increment the current score.
				++stage.score;
				// player getting point pod's sound.
				playSound(SND_POINTS, CH_POINTS);
			}
		}

		// Decrement current point pod's health by one and check if it is equal to zero.
		if (--currPointPod->health <= 0)
		{
			// Handle tail corner case.
			if (currPointPod == stage.pointPodTailPtr)
			{
				stage.pointPodTailPtr = prevPointPod;
			}
			// Delete and reset current point pod object.
			prevPointPod->next = currPointPod->next;
			free(currPointPod);
			currPointPod = prevPointPod;
		}
		prevPointPod = currPointPod;
	}
}

/**
 * @brief Create and place a new point pod object at certain position.
 * @param x An integer indicates the x position of the new point pod object.
 * @param y An integer indicates the y position of the new point pod object.
*/
static void addPointPod(int x, int y)
{
	// Create and initialize a new point pod object.
	EntityStruct* newPointPod = malloc(sizeof(EntityStruct));
	memset(newPointPod, 0, sizeof(EntityStruct));

	// Add new point pod object to the end of point pod linked list.
	stage.pointPodTailPtr->next = newPointPod;
	stage.pointPodTailPtr = newPointPod;

	// Set up the new point pod object's position.
	newPointPod->x = x;
	newPointPod->y = y;
	// Set new point pod object's x axis's delta speed(-4 to 0) and y delta speed(-4 to 4).
	newPointPod->dx = -(rand() % 5);
	newPointPod->dy = (rand() % 5) - (rand() % 5);
	// Set new point object's health(live for 10 seconds) and texture.
	newPointPod->health = FPS * 10;
	newPointPod->texture = pointPodTexture;
	// Set new point pod object's texture's width and height.
	SDL_QueryTexture(newPointPod->texture, NULL, NULL, &newPointPod->width, &newPointPod->height);

	// Shift new point pod object a little bit to the center.
	newPointPod->x -= newPointPod->width / 2;
	newPointPod->y -= newPointPod->height / 2;
}

/**
 * @brief Draw each point pod inside the point pod linked list.
*/
static void drawPointPods()
{
	for (EntityStruct* currPointPod = stage.pointPodHead.next; currPointPod != NULL; currPointPod = currPointPod->next)
	{
		blit(currPointPod->texture, currPointPod->x, currPointPod->y);
	}
}