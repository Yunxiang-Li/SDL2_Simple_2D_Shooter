#include "highscores.h"

// Forward declarations.
static void highscoreSceneLogic(void);
static void highscoreSceneDraw(void);
static void drawHighscoreTable(void);
static int highscoreComparator(const void*, const void*);

/**
 * @brief Intialize the highscore table.
*/
void initHighscoreTable()
{
	memset(&highscoreTable, 0, sizeof(HighscoreTableStruct));

	for (int i = 0; i < HIGHSCORE_TABLE_ROW_NUM; ++i)
	{
		// Scores will range from 8 to 1.
		highscoreTable.highscore[i].score = HIGHSCORE_TABLE_ROW_NUM - i;
	}
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
 * If the X key is pressed, the initStage function is called to start the game.
*/
static void highscoreSceneLogic()
{
	updateBackground();
	updateStarfield();

	if (app.keyboard[SDL_SCANCODE_X])
	{
		initStage();
	}
}

/**
 * @brief Draw the game's whole background, starfield and the highscore table.
*/
static void highscoreSceneDraw()
{
	drawBackground();
	drawStarfield();
	drawHighscoreTable();
}

/**
 * @brief Draw the whole highscore table.
*/
static void drawHighscoreTable()
{
	// Initialize the height of current row of highscore table.
	int currHeight = 150;

	// Draw the text "HIGHSCORES" near the top center position.
	drawText(425, 70, 255, 255, 255, "HIGHSCORES");

	// Traverse and draw each role of the high score table.
	for (int i = 0; i < HIGHSCORE_TABLE_ROW_NUM; ++i)
	{
		// If current score is the most recent score, then draw it in yellow.
		if (highscoreTable.highscore[i].recent)
		{
			drawText(425, currHeight, 255, 255, 0, "#%d ............. %03d", (i + 1), highscoreTable.highscore[i].score);
		}
		// Else draw it in white.
		else
		{
			drawText(425, currHeight, 255, 255, 255, "#%d ............. %03d", (i + 1), highscoreTable.highscore[i].score);
		}
		// Proceed to next row's height.
		currHeight += 50;
	}
	// Draw text to tell the player what they need to do to start the game.
	drawText(425, 600, 255, 255, 255, "PRESS FIRE(X) TO PLAY!");
}

/**
 * @brief Try to add the latest score into the highscore table.
 * @param score An integer indicates the latest score.
*/
void addHighscore(int score)
{
	// Create an array of HighScoreStruct objects with one more size than HighScoreTableStruct.
	HighScoreStruct tempHighscoreTable[HIGHSCORE_TABLE_ROW_NUM + 1];

	// Traverse and store each current high score into temp highscore table and reset recent attribute to zero.
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
	qsort(tempHighscoreTable, HIGHSCORE_TABLE_ROW_NUM + 1, sizeof(HighScoreStruct), highscoreComparator);

	// Copy first HIGHSCORE_TABLE_ROW_NUM values from temp highscore table back to current highscore table.
	for (int i = 0; i < HIGHSCORE_TABLE_ROW_NUM; ++i)
	{
		highscoreTable.highscore[i] = tempHighscoreTable[i];
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
	HighScoreStruct* h1 = ((HighScoreStruct*)p1);
	HighScoreStruct* h2 = ((HighScoreStruct*)p2);

	return h2->score - h1->score;
}