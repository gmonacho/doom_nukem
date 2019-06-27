#include "doom_nukem.h"

void    apply_damage(t_player *player, int damage)
{   
    int tmp;

    tmp = 0;
    if (player->currentArmor > 0 && damage != 0)
    {   
        if (player->currentArmor < damage)
        {
            tmp = damage - player->currentArmor;
            player->currentArmor = 0;
            player->currentHp -= tmp;
        }
        player->currentArmor -= damage;
    }
    else
        player->currentHp -= damage;
}

void    apply_heal_armor(t_player *player, int heal, int armor)
{   
    if ((player->currentArmor + armor) >= 100)
        player->currentArmor = 100;
    if ((player->currentHp + heal) >= 100)
        player->currentHp = 100;
    if (player->currentArmor < 100 && armor != 0)
        player->currentArmor += armor;
    if (player->currentHp < 100 && heal != 0)
        player->currentHp += heal; 
}


void    damage_heal(t_player *player, int damage, int armor, int heal)
{
    apply_damage(player, damage);
    apply_heal_armor(player, heal, armor);
}