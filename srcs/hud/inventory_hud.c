#include "doom_nukem.h"

void    hud_slot(t_win *win, t_texHud *texHud, t_player *player)
{
    int i;
    float slotPosx;

    i = 0;
    slotPosx = 240;
    while (i++ < 4)
    {
       SDL_RenderCopy(win->rend, texHud->tex[5], NULL, &(SDL_Rect){(slotPosx), (win->h * 0.8875), (win->w * 0.075), (win->h * 0.09375)});
       slotPosx += 80;
    }
    SDL_RenderCopy(win->rend, texHud->tex[6], NULL, &(SDL_Rect){((240 + (80 * player->selected_slot))), (win->h * 0.8875), (win->w * 0.075), (win->h * 0.09375)});
}

void    inventory_hud(t_win *win, t_texHud *texHud, t_player *player)
{
    hud_slot(win, texHud, player);
}