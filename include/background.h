#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#include <SDL.h>

#include "defs.h"
#include "structs.h"

// Extern declarations.
extern SDL_Texture* loadTexture(char*);
extern AppStruct app;

#endif // background.h