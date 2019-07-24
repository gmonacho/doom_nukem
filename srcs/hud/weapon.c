#include "doom_nukem.h"

void    print_weapon(t_win *win, t_player *player, t_texHud *texHud)
{   
    int             slotposx;

    slotposx = 500;
    if (player->inventory->weapon == 1 && !(player->magazine == 0 && player->ammo == 0))
    {   
        SDL_RenderCopy(win->rend, texHud->tex[10], NULL, &(SDL_Rect){(slotposx + 50), (440), (700), (350)});
        SDL_RenderCopy(win->rend, texHud->tex[11], NULL, &(SDL_Rect){(450), (350), (120), (120)});
    }
}