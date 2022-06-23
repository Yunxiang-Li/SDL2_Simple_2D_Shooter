// This class is responsible for handling the actual game logic.
#pragma once
#ifndef STAGE_H
#define STAGE_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#include <SDL.h>

#include "defs.h"
#include "structs.h"

extern void blit(SDL_Texture*, int, int);
extern void blitRect(SDL_Texture*, SDL_Rect*, int, int);
extern void calcSlope(int, int, int, int, float*, float*);
extern SDL_Texture* loadTexture(char*);
extern int hasCollision(SDL_Rect*, SDL_Rect*);
extern void playSound(int, int);
void drawText(int, int, int, int, int, char*, ...);

extern AppStruct app;
extern StageStruct stage;

#endif // STAGE_H