#include "doom_nukem.h"

void    print_weapon(t_win *win, t_player *player, t_texHud *texHud)
{   
    int             slotposx;

    slotposx = 500;
    if (player->inventory->weapon == 1 && !(player->magazine == 0 && player->ammo == 0))
    {   
        SDL_RenderCopy(win->rend, texHud->tex_weapon[player->timers.bullet_cd.index], NULL, &(SDL_Rect){(slotposx + 50), (440), (700), (370)});
        SDL_RenderCopy(win->rend, texHud->tex[11], NULL, &(SDL_Rect){(450), (400), (120), (120)});
    }
    else
    {
        SDL_RenderCopy(win->rend, texHud->tex_reload[player->timers.reload_cd.index], NULL, &(SDL_Rect){(slotposx + 50), (460), (700), (350)});   
    }
}