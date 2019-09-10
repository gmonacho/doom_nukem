#include "doom_nukem.h"

void    all_reload(t_player *player)
{
    int nb;

    nb = 30 - player->ammo;
    if (player->magazine - nb > 0)
    {
        player->magazine -= nb;
        player->ammo += nb;
    }
    else
    {
        player->ammo += player->magazine;
        player->magazine = 0;
    }
}

void    reload_ammo(t_player *player)
{   
    all_reload(player);
}