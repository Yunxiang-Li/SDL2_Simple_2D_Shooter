#include "util.h"

/**
 * @brief Check if two input SDL_Rect objects are overlapping.
 * @param rect1 The first input SDL_Rect object
 * @param rect2 The second input SDL_Rect object
 * @return 1 for overlapping and 0 for non-overlapping.
*/
int collision(SDL_Rect* rect1, SDL_Rect* rect2)
{
	return (MAX(rect1->x, rect2->x) < MIN(rect1->x + rect1->w, rect2->x + rect2->w)) &&
		(MAX(rect1->y, rect2->y) < MIN(rect1->y + rect1->h, rect2->y + rect2->h));
}
