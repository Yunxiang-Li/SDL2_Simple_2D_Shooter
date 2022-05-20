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

extern void blit(SDL_Texture* texture, int x, int y);
extern void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);
extern void calcSlope(int x1, int y1, int x2, int y2, float* dx, float* dy);
extern SDL_Texture* loadTexture(char* filename);
extern int collision(SDL_Rect* rect1, SDL_Rect* rect2);
extern void playSound(int id, int channel);

extern AppStruct app;
extern StageStruct stage;

#endif // STAGE_H