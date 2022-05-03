#include "input.h"

// Forward declarations.
void doKeyDown(SDL_KeyboardEvent*);
void doKeyUp(SDL_KeyboardEvent*);

/**
 * @brief Basic input handling.
 *
 */
void doInput()
{
    SDL_Event event;

    // Check if pending events exist.
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
            // Check if the SDL window is closed.
            case SDL_QUIT:
                exit(0);
                break;
            // Check if one key is down.
            case SDL_KEYDOWN:
                doKeyDown(&(event.key));
                break;
            // Check if one key is up.
            case SDL_KEYUP:
                doKeyUp(&(event.key));
                break;
            default:
                break;

        }
    }
}

/**
 * @brief Handle non-repeated key down events.
 * @param eventPtr A pointer to SDL_KeyboardEvent object.
*/
void doKeyDown(SDL_KeyboardEvent* eventPtr)
{
    // If this keyevent is not a repeat and keycode is inside MAX_KEYBOARD_KEYS.
    if (eventPtr->repeat == 0 && eventPtr->keysym.scancode < MAX_KEYBOARD_KEYS && eventPtr->keysym.scancode >= 0)
    {
        // Set up the relative state's condition.
        app.keyboard[eventPtr->keysym.scancode] = 1;
    }
}

/**
 * @brief Handle non-repeated key up events.
 * @param eventPtr A pointer to SDL_KeyboardEvent object.
*/
void doKeyUp(SDL_KeyboardEvent* eventPtr)
{
    // If this keyevent is not a repeat and keycode is inside MAX_KEYBOARD_KEYS.
    if (eventPtr->repeat == 0 && eventPtr->keysym.scancode < MAX_KEYBOARD_KEYS && eventPtr->keysym.scancode >= 0)
    {
        // Reset the relative state's condition.
        app.keyboard[eventPtr->keysym.scancode] = 0;
    }
}