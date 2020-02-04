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
	if (poly->object->type == HEAL)
	{
		player->inventory->item[0]->nb++;
		printf("Popo heal +1\n");
	}
	if (poly->object->type == ARMOR)
	{
		player->inventory->item[1]->nb++;
		printf("Popo armor +1\n");
	}
	if (poly->object->type == TP)
		tp(map, player, map->polys);
	if (poly->object->type == GUN)
		player->inventory->weapon = 1;
	if (poly->object->type == BULLET)
		player->inventory->magazine += 20;

}