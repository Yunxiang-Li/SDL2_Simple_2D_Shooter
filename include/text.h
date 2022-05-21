// This class contains all information about in-game text.
#pragma once

#ifndef TEXT_H
#define TEXT_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#include "SDL.h"

#include "defs.h"
#include "structs.h"

extern SDL_Texture* loadTexture(char*);
extern void blitRect(SDL_Texture*, SDL_Rect*, int, int);

// Define each letter's width and height.
#define LETTER_HEIGHT    28
#define LETTER_WIDTH     18

#endif // TEXT_H