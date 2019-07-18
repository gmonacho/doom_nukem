#include "doom_nukem.h"
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
void    reload_ammo(t_player *player)
{
    if ((player->magazine - 30) > 0)  
        big_size(player);
    else 
        little_size(player); 
}