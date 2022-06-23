#include "util.h"

/**
 * @brief Check if two input SDL_Rect objects are overlapping.
 * @param rect1 The first input SDL_Rect object
 * @param rect2 The second input SDL_Rect object
 * @return 1 for overlapping and 0 for non-overlapping.
*/
int hasCollision(SDL_Rect* rect1, SDL_Rect* rect2)
{
	return (MAX(rect1->x, rect2->x) < MIN(rect1->x + rect1->w, rect2->x + rect2->w)) &&
		(MAX(rect1->y, rect2->y) < MIN(rect1->y + rect1->h, rect2->y + rect2->h));
}

/**
 * @brief Calculate the normalized step from one point to another.
 * @param x1 An integer indicates the X position of first point.
 * @param y1 An integer indicates the Y position of first point.
 * @param x2 An integer indicates the X position of second point.
 * @param y2 An integer indicates the Y position of secibd point.
 * @param dx A pointer to float indicates the expected normalized delta movement on X axis (can be -1, 0 or 1).
 * @param dy A pointer to float indicates the expected normalized delta movement on Y axis (can be -1, 0 or 1).
*/
void calcSlope(int x1, int y1, int x2, int y2, float* dx, float* dy)
{
	int steps = MAX(abs(x1 - x2), abs(y1 - y2));

	if (steps == 0)
	{
		*dx = *dy = 0;
		return;
	}

	*dx = (x1 - x2);
	*dx /= steps;

	*dy = (y1 - y2);
	*dy /= steps;
}
