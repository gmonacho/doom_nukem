#include "doom_nukem.h"

void    start_cooldown(t_timer *timer, uint32_t time)
{   
    timer->time = time;
    timer->save = SDL_GetTicks();
}

int     test_timer(t_timer *timer)
{   
    if (SDL_GetTicks() > timer->save + timer->time)
    {
        timer->save = SDL_GetTicks();
        return (1);
    }
    else
        return (0);
}