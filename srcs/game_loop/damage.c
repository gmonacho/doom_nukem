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

void    use_item(t_player *player, t_music *music, int slotSelected)
{
    if (slotSelected == 0 && player->inventory->item[0]->nb > 0)
    {
        player->inventory->item[0]->nb -= 1;
         Mix_PlayChannel(4, music->tmusic[2], 0);
        apply_heal_armor(player, 15, 0);
    }
    if (slotSelected == 1 && player->inventory->item[1]->nb > 0)
    {
        player->inventory->item[1]->nb -= 1;
         Mix_PlayChannel(5, music->tmusic[3], 0);
        apply_heal_armor(player, 0, 15);
    }
    if (slotSelected == 2 && player->inventory->item[2]->nb > 0)
        player->inventory->item[2]->nb -= 1;
}

void    damage_heal(t_player *player, int damage, int armor, int heal)
{
    apply_damage(player, damage);
    apply_heal_armor(player, heal, armor);
}