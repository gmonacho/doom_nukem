#include "doom_nukem.h"

void    print_weapon(t_win *win, t_player *player, t_texHud *texHud)
{   
    int             slotposx;

    slotposx = 500;
    if (player->inventory->weapon == 1 && !(player->magazine == 0 && player->ammo == 0))
    {   
        SDL_RenderCopy(win->rend, texHud->tex_weapon[player->timers.bullet_cd.index], NULL, &(SDL_Rect){(win->w * 0.55), (win->h * 0.55), (win->w * 0.7), (win->h * 0.4625)});
        SDL_RenderCopy(win->rend, texHud->tex[11], NULL, &(SDL_Rect){(win->w * 0.45), (win->h * 0.5), (win->w * 0.12), (win->h * 0.15)});
    }
    else
    {
        SDL_RenderCopy(win->rend, texHud->tex_reload[player->timers.reload_cd.index], NULL, &(SDL_Rect){(win->w * 0.55), (win->h * 0.575), (win->w * 0.7), (win->h * 0.4375)});   
    }
}