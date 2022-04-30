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

#define PLAYER_WIDTH 48
#define PLAYER_HEIGHT 46
#define BULLET_WIDTH 5
#define BULLET_HEIGHT 5

// Declare all current functions(which are all defined somewhere else).
extern void cleanup(void);
extern void doInput(void);
extern void initSDL(void);
extern void prepareScene(void);
extern void presentScene(void);
extern SDL_Texture* loadTexture(char*);
extern void blit(SDL_Texture*, int, int);

// Define app, bullet and player here.
AppStruct app;
EntityStruct player;
EntityStruct bullet;

#endif // MAIN_H