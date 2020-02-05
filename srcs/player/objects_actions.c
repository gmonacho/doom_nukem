#include "doom_nukem.h"

static void			tp(t_map *map, t_player *player, t_poly *polys)
{
	map = NULL;
	player = NULL;
	polys = NULL;
}

void				drop_box(t_map *map, t_player *player)
{
	t_object		*object;
	t_poly			*poly;
	t_fdot_3d		translate;

	object = map->object;
	while (object)
	{
		if (object->type == BOX && !object->visible && !object->collide)
		{
			poly = object->poly;
			translate = fdot_3d_sub((t_fdot_3d){player->width_2 + object->width_2 + 10, 0, 0}, object->pos);
			while (poly)
			{
				translate_dot(&(poly->dots_rotz_only[0]), translate);
				translate_dot(&(poly->dots_rotz_only[1]), translate);
				translate_dot(&(poly->dots_rotz_only[2]), translate);
				translate_dot(&(poly->dots_rotz_only[3]), translate);
				poly->equation_rotz_only.d = -(poly->equation_rotz_only.v.x * poly->dots_rotz_only[0].x +\
												poly->equation_rotz_only.v.y * poly->dots_rotz_only[0].y +\
												poly->equation_rotz_only.v.z * poly->dots_rotz_only[0].z);
			}
			object->collide = 1;
			object->visible = 1;
			return ;
		}
		object = object->next;
	}
}

static void			rotate_all_objects(t_player *player, t_object *object)
{
	t_fdot_3d	pos;
	// t_poly		*poly;

	while (object)
	{
		if (object->type != BOX)
		{
			pos = mid_segment(object->poly->dots_rotz_only[0], object->poly->dots_rotz_only[2]);
			object->poly->equation_rotz_only.v = rotate_dot(object->poly->equation_rotz_only.v, player->rz);
			object->poly->dots_rotz_only[0] = fdot_3d_add(rotate_dot(fdot_3d_sub(object->poly->dots_rotz_only[0], pos), player->rz), pos);
			object->poly->dots_rotz_only[1] = fdot_3d_add(rotate_dot(fdot_3d_sub(object->poly->dots_rotz_only[1], pos), player->rz), pos);
			object->poly->dots_rotz_only[2] = fdot_3d_add(rotate_dot(fdot_3d_sub(object->poly->dots_rotz_only[2], pos), player->rz), pos);
			object->poly->dots_rotz_only[3] = fdot_3d_add(rotate_dot(fdot_3d_sub(object->poly->dots_rotz_only[3], pos), player->rz), pos);
		}
		object = object->next;
	}
	// while (object)
	// {
	// 	poly = object->poly;
	// 	if (object->type == BOX)
	// 		pos = object->pos;
	// 	else
	// 		pos = mid_segment(poly->dots_rotz_only[0], poly->dots_rotz_only[2]);
	// 	while (poly)
	// 	{
	// 		poly->equation_rotz_only.v = rotate_dot(poly->equation_rotz_only.v, player->rz);
	// 		poly->dots_rotz_only[0] = fdot_3d_add(rotate_dot(fdot_3d_sub(poly->dots_rotz_only[0], pos), player->rz), pos);
	// 		poly->dots_rotz_only[1] = fdot_3d_add(rotate_dot(fdot_3d_sub(poly->dots_rotz_only[1], pos), player->rz), pos);
	// 		poly->dots_rotz_only[2] = fdot_3d_add(rotate_dot(fdot_3d_sub(poly->dots_rotz_only[2], pos), player->rz), pos);
	// 		poly->dots_rotz_only[3] = fdot_3d_add(rotate_dot(fdot_3d_sub(poly->dots_rotz_only[3], pos), player->rz), pos);
	// 		poly = poly->next;
	// 	}
	// 	object = object->next;
	// }
}

int				objects_actions(t_map *map, t_player *player, t_poly *poly)
{
	if (poly->object->type == HEAL)
	{
		player->inventory->item[0]->nb++;
		// printf("Popo heal +1\n");
	}
	if (poly->object->type == ARMOR)
	{
		player->inventory->item[1]->nb++;
		// printf("Popo armor +1\n");
	}
	if (poly->object->type == TP)
		tp(map, player, map->polys);
	if (poly->object->type == GUN)
		player->inventory->weapon = 1;
	if (poly->object->type == BULLET)
		player->inventory->magazine += 20;
	if (poly->object->type == GRAVITY_INV)
	{
		map->gravity = -map->gravity;
		start_cooldown(&(map->gravity_inv_time), 10000);
	}
	if (poly->object->type == BOX)
	{
		if (player->interaction_inventaire)
		{
			// player->inventory->item[3]->nb++;
		}
		else
			return (0);
	}
	poly->object->collide = 0;
	poly->object->visible = 0;
	return (1);
}

void			objects_animations(t_map *map, t_player *player, t_object *object)
{
	if (test_timer_refresh(&(map->objects_animation)))
		rotate_all_objects(player, object);
	if (map->gravity < 0 && test_timer(&(map->gravity_inv_time)))
		map->gravity = -map->gravity;
}