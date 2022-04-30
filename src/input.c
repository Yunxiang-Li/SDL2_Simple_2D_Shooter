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
    while (SDL_PollEvent(&event))
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
    // First check if this is not a repeat.
    if (eventPtr->repeat == 0)
    {   
        switch (eventPtr->keysym.scancode)
        {
            // Check up, down, left, right arrow key cases.
            case SDL_SCANCODE_UP:
                app.up = 1;
                break;
            case SDL_SCANCODE_DOWN:
                app.down = 1;
                break;
            case SDL_SCANCODE_LEFT:
                app.left = 1;
                break;
            case SDL_SCANCODE_RIGHT:
                app.right = 1;
                break;

            // Check if player is currently firing a bullet or not(Press X key).
            case SDL_SCANCODE_X:
                app.fire = 1;
                break;
            default:
                break;
        }
    }
}

/**
 * @brief Handle non-repeated key up events.
 * @param eventPtr A pointer to SDL_KeyboardEvent object.
*/
void doKeyUp(SDL_KeyboardEvent* eventPtr)
{
    // First check if this is not a repeat.
    if (eventPtr->repeat == 0)
    {
        // Check up, down, left, right arrow key cases.
        switch (eventPtr->keysym.scancode)
        {
        case SDL_SCANCODE_UP:
            app.up = 0;
            break;
        case SDL_SCANCODE_DOWN:
            app.down = 0;
            break;
        case SDL_SCANCODE_LEFT:
            app.left = 0;
            break;
        case SDL_SCANCODE_RIGHT:
            app.right = 0;
            break;

        // Check if player is stop firing a bullet or not(Release X key).
        case SDL_SCANCODE_X:
            app.fire = 0;
            break;
        default:
            break;
        }
    }
}