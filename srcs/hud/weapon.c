#include "doom_nukem.h"

void    print_weapon(t_win *win, t_player *player, t_texHud *texHud)
{   
    SDL_Texture     *text;
    char            *tmp;
    int             slotposx;

    slotposx = 500;
    tmp = ft_strdup("EMPTY AMMO PRESS 'R' ");
    if (player->inventory->weapon == 1 && !(player->magazine == 0 && player->ammo == 0))
    {
        SDL_RenderCopy(win->rend, texHud->tex[10], NULL, &(SDL_Rect){(slotposx + 50), (440), (700), (350)});
        SDL_RenderCopy(win->rend, texHud->tex[11], NULL, &(SDL_Rect){(450), (350), (120), (120)});
        if (player->ammo == 0 && !(test_timer(&(player->timers.text_cd)) == 1))
        {
            text = generate_text(win->rend, texHud->police, tmp, (SDL_Color){200, 0, 2, 40});
            SDL_RenderCopy(win->rend, text, NULL, &(SDL_Rect){(300), (650), (250), (50)});
        }
    }
    free(tmp);
}