// This class handles the initialization and cleanup of the game.
#pragma once
#ifndef INIT_H
#define INIT_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "structs.h"
#include "SDL_mixer.h"

// Extern declarations.
extern void initBackground(void);
extern void initFonts(void);
extern void initHighscoreTable(void);
extern void initSounds(void);
extern void initStarfield(void);
extern void loadMusic(char*);
extern void playMusic(int);

// Declare an app here to help initialize the game.
extern AppStruct app;

#endif // INIT_H