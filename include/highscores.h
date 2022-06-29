// Include all features related to highscores.
#pragma once
#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#include <SDL.h>

#include "defs.h"
#include "structs.h"

// Declare the app and highscore table here to help process the highscore related features.
extern AppStruct app;
extern HighscoreTableStruct highscoreTable;

// Extern functions declarations.
extern void updateBackground(void);
extern void updateStarfield(void);
extern void drawBackground(void);
extern void drawStarfield(void);
extern void drawText(int x, int y, int r, int g, int b, char* format, ...);
extern void initStage(void);

#endif // HIGHSCORES_H

