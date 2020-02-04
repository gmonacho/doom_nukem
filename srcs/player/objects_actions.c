#include "doom_nukem.h"

void			tp(t_map *map, t_player *player, t_poly *polys)
{
	map = NULL;
	player = NULL;
	polys = NULL;
}

void			objects_actions(t_map *map, t_player *player, t_poly *poly)
{
	poly->visible = 0;
	poly->collide = 0;
	if (poly->object == HEALTH)
	{
		player->inventory->item[0]->nb++;
		printf("Popo heal +1\n");
	}
	if (poly->object == ARMOR)
	{
		player->inventory->item[1]->nb++;
		printf("Popo armor +1\n");
	}
	if (poly->object == TP)
		tp(map, player, map->polys);
	if (poly->object == GUN)
		player->inventory->weapon = 1;
	if (poly->object == BULLET)
		player->inventory->magazine += 20;

}