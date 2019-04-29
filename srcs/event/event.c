#include "doom_nukem.h"

int		key_pressed(Uint32 sdl_keycode)
{
    static Uint8    *key_state = NULL;

    if (!key_state)
        key_state = (Uint8*)SDL_GetKeyboardState(NULL);
    return (key_state[sdl_keycode] ? 1 : 0);
}