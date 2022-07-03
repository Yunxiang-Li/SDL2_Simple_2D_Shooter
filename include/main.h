// This class responsible for the entrance and the main game loop.
#pragma once
#ifndef MAIN_H
#define MAIN_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"
#include <SDL.h>
#include "structs.h"
#include "defs.h"

// Declare all current functions(which are all defined somewhere else).
extern void cleanup(void);
extern void doInput(void);
extern void initSDL(void);
extern void switchToStage(void);
extern void prepareScene(void);
extern void presentScene(void);
extern void initSounds(void);
extern void initFonts(void);
extern void initGame(void);
extern void switchToHighscoreScene(void);
extern void switchToTitleScreen(void);

// Create app, stage and highscore table.
AppStruct app;
StageStruct stage;
HighscoreTableStruct highscoreTable;

#endif // MAIN_H