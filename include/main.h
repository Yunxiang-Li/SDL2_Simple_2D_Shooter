#pragma once
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

// Define appStruct here.
AppStruct appStruct;