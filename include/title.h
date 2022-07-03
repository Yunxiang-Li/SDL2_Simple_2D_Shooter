#pragma once
#ifndef TITLE_H
#define TITLE_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#include "SDL.h"

#include "defs.h"
#include "structs.h"

// Extern declarations.
extern AppStruct app;
extern SDL_Texture* loadTexture(char*);
extern void updateBackground(void);
extern void updateStarfield(void);
extern void switchToHighscoreScene(void);
extern void switchToStage(void);
extern void drawBackground(void);
extern void drawStarfield(void);
extern void blitRect(SDL_Texture*, SDL_Rect*, int, int);
extern void drawText(int, int, int, int, int, int, char*, ...);

#endif // !TITLE_H