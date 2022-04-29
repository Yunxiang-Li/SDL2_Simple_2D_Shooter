#include "input.h"

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
        default:
            break;

        }

    }

}