#include "doom_nukem.h"

void    print_no_ammo(t_win *win)
{
    char        *tmp;
    SDL_Texture *text;

    tmp = ft_strdup("NO AMMO LEFT");
    text = generate_text(win->rend, win->texHud->police, tmp, (SDL_Color){200, 0, 2, 40});
    SDL_RenderCopy(win->rend, text, NULL, &(SDL_Rect){(300), (650), (250), (50)});
    free(tmp);
}

void    big_size(t_player *player)
{   
    int nb;

    nb = 0;
    if (player->ammo == 0)
    {
        player->magazine -= 30;
        player->ammo = 30;
    }
    if (player->ammo > 0)
    {
        nb = player->ammo;
        player->ammo = 30;
       player->magazine -= (30 - nb);
    }     
}

void    little_size(t_player *player)
{   
    int nb;
    int limit;

    nb = 0;
    limit = 30;
    nb = limit - player->ammo; 
    if (player->magazine > nb)
    {
        player->magazine -= nb;
        player->ammo += nb;
    }
    if (player->magazine < nb)
    {
        player->ammo += player->magazine;
        player->magazine = 0;
    }
}
void    reload_ammo(t_win *win, t_player *player)
{   
    if ((player->magazine - 29) >= 0)  
        big_size(player);
    else 
        little_size(player); 
    if (player->magazine == 0)
        print_no_ammo(win);
}