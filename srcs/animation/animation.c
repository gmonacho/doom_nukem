#include "doom_nukem.h"

void    main_animation(t_map *map, t_win *win, t_texHud *texHud)
{
    if (test_timer(&(map->player.timers.item_cd)) == 0)
    {
        SDL_RenderCopy(win->rend, texHud->tex[12], NULL, &(SDL_Rect){(win->w * 0.01), (win->h * 0.8875), (win->w * 0.075), (win->h * 0.09375)});
    }
    else
    {
        SDL_RenderCopy(win->rend, texHud->tex[13], NULL, &(SDL_Rect){(win->w * 0.01), (win->h * 0.8875), (win->w * 0.075), (win->h * 0.09375)});
    }
}