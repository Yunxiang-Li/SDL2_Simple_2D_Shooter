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

// Declare all current functions(which are all defined somewhere else).
extern void cleanup(void);
extern void doInput(void);
extern void initSDL(void);
extern void prepareScene(void);
extern void presentScene(void);
extern SDL_Texture* loadTexture(char*);
extern void blit(SDL_Texture*, int, int);

// Define app and player here.
AppStruct app;
PlayerStruct player;

#endif // MAIN_H