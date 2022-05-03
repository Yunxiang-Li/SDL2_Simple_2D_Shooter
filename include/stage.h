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
extern SDL_Texture* loadTexture(char* filename);

extern AppStruct app;
extern StageStruct stage;

#endif // STAGE_H