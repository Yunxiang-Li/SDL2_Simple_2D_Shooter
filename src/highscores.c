#include "highscores.h"

// Forward declarations.
static void highscoreSceneLogic(void);
static void highscoreSceneDraw(void);
static void drawHighscoreTable(void);
static int highscoreComparator(const void*, const void*);
static void drawInputNameScene(void);
static void updateInputName(void);

// Declare a HighscoreStruct pointer indicates one new highscore.
static HighscoreStruct* newHighscorePtr;
// Declare a integer indicates the timer which controls if the cursor is blink or not.
static int cursorBlinkTimer;

/**
 * @brief Intialize the highscore table.
*/
void initHighscoreTable()
{
	memset(&highscoreTable, 0, sizeof(HighscoreTableStruct));

	for (int i = 0; i < HIGHSCORE_TABLE_ROW_NUM; ++i)
	{
		// Each score will be updated as the default value(0).
		highscoreTable.highscore[i].score = 0;
		// Each player name will be updated as default name("ANONYMOUS").
		strncpy(highscoreTable.highscore[i].nameArray, "ANONYMOUS", MAX_SCORE_NAME_LENGTH);
		highscoreTable.highscore[i].nameArray[MAX_SCORE_NAME_LENGTH - 1] = '\0';
	}

	// Initialize new highscore pointer and cursor blink timer.
	newHighscorePtr = NULL;
	cursorBlinkTimer = 0;
}

/**
 * @brief Assign our delegate function (highscore_logic and highscore_draw) pointers to locally declared app object and clears its keyboard entry
 * to prevent the game from jumping between the highscore table scene and the game scene itself.
*/
void initHighscores()
{
	app.delegate.logic = highscoreSceneLogic;
	app.delegate.draw = highscoreSceneDraw;

	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);
}

/**
 * @brief A bound callback to handle all in game updates.Handles updates of the background and starfield.
*/
static void highscoreSceneLogic()
{
	updateBackground();
	updateStarfield();

	// If player gains a new highscore, then update input name.
	if (newHighscorePtr != NULL)
	{
		updateInputName();
	}
	// If the X key is pressed, the initStage function is called to start the game.
	else if (app.keyboard[SDL_SCANCODE_X])
	{
		initStage();
	}

	// Reset cursor blink timer to zero after 1 second.
	if (++cursorBlinkTimer >= FPS)
	{
		cursorBlinkTimer = 0;
	}
}

/**
 * @brief Draw the game's whole background, starfield and the highscore part.
*/
static void highscoreSceneDraw()
{
	drawBackground();
	drawStarfield();

	// Draw name input scene if a new highscore exist.
	if (newHighscorePtr != NULL)
	{
		drawInputNameScene();
	}
	// Otherwise draw the highscore table.
	else
	{
		drawHighscoreTable();
	}
}

/**
 * @brief Draw player name input scene.
*/
static void drawInputNameScene()
{
	// Draw helper messages and player input name.
	drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255, TEXT_CENTER, "CONGRATULATIONS, YOU'VE GAINED A HIGHSCORE!");
	drawText(SCREEN_WIDTH / 2, 120, 255, 255, 255, TEXT_CENTER, "ENTER YOUR NAME BELOW:");
	drawText(SCREEN_WIDTH / 2, 250, 128, 255, 128, TEXT_CENTER, newHighscorePtr->nameArray);

	// Draw cursor blink effect(the cursor blinks 1 time per 30 frames).
	if (cursorBlinkTimer < FPS / 2)
	{
		// The cursor should be a small green rectangle after the last player name's letter.
		SDL_Rect r;
		r.x = ((SCREEN_WIDTH / 2) + (strlen(newHighscorePtr->nameArray) * LETTER_WIDTH) / 2) + 5;
		r.y = 250;
		r.w = LETTER_WIDTH;
		r.h = LETTER_HEIGHT;

		// Set cursor color to be green.
		SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
		// Render the cursor.
		SDL_RenderFillRect(app.renderer, &r);
	}

	// Draw message to helper player quit the input scene.
	drawText(SCREEN_WIDTH / 2, 625, 255, 255, 255, TEXT_CENTER, "PRESS ENTER WHEN FINISHED");
}

/**
 * @brief Draw the whole highscore table.
*/
static void drawHighscoreTable()
{
	// Initialize the height of current row of highscore table.
	int currHeight = 150;

	// Draw the text "HIGHSCORES" near the top center position.
	drawText(SCREEN_WIDTH/2, 70, 255, 255, 255, TEXT_CENTER, "HIGHSCORES");
	// Declare 3 integers to hold rgb values of each high score row.
	int r, g, b;
	// Traverse and draw each role of the high score table.
	for (int i = 0; i < HIGHSCORE_TABLE_ROW_NUM; ++i)
	{
		// Initialize rgb values.
		r = 255;
		g = 255;
		b = 255;

		// If current score is the most recent score, then set blue to zero.
		if (highscoreTable.highscore[i].recent)
		{
			b = 0;
		}
		// Draw each row, align player name to the left.
		drawText(SCREEN_WIDTH / 2, currHeight, r, g, b, TEXT_CENTER, "#%d. %-31s ............. %03d",
			(i + 1), highscoreTable.highscore[i].nameArray, highscoreTable.highscore[i].score);
		// Update to next row's height.
		currHeight += 50;
	}
	// Draw text to tell the player what they need to do to start the game.
	drawText(SCREEN_WIDTH/2, 600, 255, 255, 255, TEXT_CENTER, "PRESS FIRE(X) TO PLAY!");
}

/**
 * @brief Try to add the latest score into the highscore table.
 * @param score An integer indicates the latest score.
*/
void addHighscore(int score)
{
	// Create an array of HighScoreStruct objects with one more size than HighScoreTableStruct.
	HighscoreStruct tempHighscoreTable[HIGHSCORE_TABLE_ROW_NUM + 1];

	// Initialize each attribute to zero.
	memset(tempHighscoreTable, 0, sizeof(HighscoreStruct) * (HIGHSCORE_TABLE_ROW_NUM + 1));

	// Traverse and store each current high score into temp highscore table and reset each recent attribute to zero.
	for (int i = 0; i < HIGHSCORE_TABLE_ROW_NUM; ++i)
	{
		tempHighscoreTable[i] = highscoreTable.highscore[i];
		tempHighscoreTable[i].recent = 0;
	}

	// Store the input score as the last element and set its recent attribute to 1(indicates latest score).
	tempHighscoreTable[HIGHSCORE_TABLE_ROW_NUM].score = score;
	tempHighscoreTable[HIGHSCORE_TABLE_ROW_NUM].recent = 1;

	// Sorts the (HIGHSCORE_TABLE_ROW_NUM + 1) elements of the array pointed to by tempHighscoreTable.
	// Each element has sizeof(HighScoreStruct) bytes length, using the highscoreComparator to determine the order.
	qsort(tempHighscoreTable, HIGHSCORE_TABLE_ROW_NUM + 1, sizeof(HighscoreStruct), highscoreComparator);

	// Reset to NULL first.
	newHighscorePtr = NULL;
	// Copy first HIGHSCORE_TABLE_ROW_NUM values from temp highscore table back to current highscore table.
	for (int i = 0; i < HIGHSCORE_TABLE_ROW_NUM; ++i)
	{
		highscoreTable.highscore[i] = tempHighscoreTable[i];
		// Store the most recent highscore.
		if (highscoreTable.highscore[i].recent)
		{
			newHighscorePtr = &highscoreTable.highscore[i];
		}
	}
}

/**
 * @brief Creates a comparator to help set HighScoreStructs object with larger values at front part of the highscore table.
 * @param p1 A const void pointer indicates the first HighScoreStruct pointer to be compared. 
 * @param p2 A const void pointer indicates the second HighScoreStruct pointer to be compared.
 * @return An integer indicates two HighScoreStruct objects' relaton.
 * (<0 indicates p1 should place before p2, >0 indicates p2 should place before p1, =0 indicates p1 is equivalent to p2)
*/
static int highscoreComparator(const void* p1, const void* p2)
{
	HighscoreStruct* h1 = ((HighscoreStruct*)p1);
	HighscoreStruct* h2 = ((HighscoreStruct*)p2);

	return h2->score - h1->score;
}

/**
 * @brief Process user input as player name.
*/
static void updateInputName()
{
	// Retrieve new highscore's player name array's length.
	int n = strlen(newHighscorePtr->nameArray);
	char c;
	// Traverse and process each char from app's text array.
	for (int i = 0; i < strlen(app.inputTextArray); ++i)
	{	
		// Uppercase each char.
		c = toupper(app.inputTextArray[i]);

		// Check if current char is within possible range of new highscore's player name.
		if (n < MAX_SCORE_NAME_LENGTH - 1 && c >= ' ' && c <= 'Z')
		{
			// Assign current char back to new highscore's player name and proceed to next char.
			newHighscorePtr->nameArray[n++] = c;
		}
	}

	// If player presses backspace key, remove the most recent char and reset backspace key state.
	if (n > 0 && app.keyboard[SDL_SCANCODE_BACKSPACE])
	{
		newHighscorePtr->nameArray[--n] = '\0';
		app.keyboard[SDL_SCANCODE_BACKSPACE] = 0;
	}

	// If player presses enter key, then handle no input case and set new highscore struct's pointer back to NULL.
	if (app.keyboard[SDL_SCANCODE_RETURN])
	{
		if (strlen(newHighscorePtr->nameArray) == 0)
		{
			strncpy(newHighscorePtr->nameArray, "ANONYMOUS", MAX_SCORE_NAME_LENGTH);
			newHighscorePtr->nameArray[MAX_SCORE_NAME_LENGTH - 1] = '\0';
		}
		newHighscorePtr = NULL;
	}
}