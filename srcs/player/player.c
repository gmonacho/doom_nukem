#include "doom_nukem.h"

void    apply_damage(t_map *map, t_player *player, int damage)
{
	Mix_PlayChannel(4, map->music->tmusic[5], 0);
    player->currentArmor -= damage;
	if (player->currentArmor < 0)
	{
		player->currentHp += player->currentArmor;
		player->currentArmor = 0;
	}
}

void    apply_heal(t_player *player, int heal)
{   
	player->currentHp += heal;
    if (player->currentHp > 100)
        player->currentHp = 100;
}

void    apply_armor(t_player *player, int armor)
{   
	player->currentArmor += armor;
    if (player->currentArmor > 100)
        player->currentArmor = 100;
}

void    use_item(t_player *player, t_music *music, int slotSelected)
{
    if (slotSelected == 0 && player->inventory->item[0]->nb > 0)
    {
        player->inventory->item[0]->nb -= 1;
        Mix_PlayChannel(4, music->tmusic[2], 0);
        apply_heal(player, 15);
    }
    if (slotSelected == 1 && player->inventory->item[1]->nb > 0)
    {
        player->inventory->item[1]->nb -= 1;
        Mix_PlayChannel(5, music->tmusic[3], 0);
        apply_armor(player, 15);
    }
    if (slotSelected == 2 && player->inventory->item[2]->nb > 0)
        player->inventory->item[2]->nb -= 1;
}
