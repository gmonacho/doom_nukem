#include "doom_nukem.h"

static void			tp(t_map *map, t_player *player, t_poly *polys)
{

	map = NULL;
	player = NULL;
	polys = NULL;
}

static t_object		*catch_box(t_player *player, t_object *object)
{
	t_fdot_3d		collision;
	t_fdot			coord;
	t_poly			*poly;
	t_cartesienne	ray;

	ray = (t_cartesienne){0, 0, 0, 1, 0, 0, 0, NULL, 0, NULL};
	while (object)
	{
		poly = object->poly;
		while (object->visible && poly)
		{
			if (intersection_plan_my_ray(&collision, poly->equation, &ray) &&\
				is_in_poly(poly, &coord, collision) &&\
				mag(collision) < player->width * 2)
				return (object);
			poly = poly->next;
		}
		object = object->next;
	}
	return (NULL);
}

void				drop_box(t_map *map, t_player *player)
{
	t_object		*object;
	// t_poly			*poly;
	// t_fdot_3d		translate;
	// t_fdot_3d		tmp;
	// int				i;

	// printf("Drop box !!!\n");
	object = map->objects;
	while (object)
	{
		if (object->type == BOX && !object->visible && !object->collide)
		{
			rotate_box(map, player, object);
			// translate_box(map, object);
			// printf("Find box ramasse a : %f %f %f\n", object->pos.x, object->pos.y, object->pos.z);
			// object->pos = fdot_3d_add(object->pos, );
			// printf("Find box ramasse a : %f %f %f\n", object->pos.x, object->pos.y, object->pos.z);
			object->collide = 1;
			object->visible = 1;
			return ;
		}
		object = object->next;
	}
	player = NULL;
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

int				objects_actions(t_map *map, t_player *player, t_object *object)
{
	// printf("Object : %d\n", object->type);
	if (object->type == HEAL)
	{
		player->inventory->item[0]->nb++;
		// printf("Popo heal +1\n");
	}
	if (object->type == ARMOR)
	{
		player->inventory->item[1]->nb++;
		// printf("Popo armor +1\n");
	}
	if (object->type == TP)
		tp(map, player, map->polys);
	if (object->type == GUN)
		player->inventory->weapon = 1;
	if (object->type == BULLET)
		player->inventory->item[3]->nb += 1;
	if (object->type == GRAVITY_INV)
	{
		map->gravity = -map->gravity;
		start_cooldown(&(map->gravity_inv_time), 10000);
	}
	// if (object->type == BOX)
	// {
	// 	if (player->interaction_inventaire)
	// 	{
	// 		player->inventory->item[4]->nb++;
	// 		object->rot_y_save = player->rot_y;
	// 	}
	// 	else
	// 		return (0);
	// }
	object->collide = 0;
	object->visible = 0;
	return (1);
}

void			objects_movements(t_map *map, t_player *player, t_object *objects)
{
	t_object	*object;

	if (test_timer_refresh(&(map->objects_animation)))
		rotate_all_objects(player, objects);
	if (map->gravity < 0 && test_timer(&(map->gravity_inv_time)))
		map->gravity = -map->gravity;
	if (player->interaction_inventaire && (object = catch_box(player, objects)))
	{
		player->inventory->item[4]->nb++;
		object->rot_y_save = player->rot_y;
		object->collide = 0;
		object->visible = 0;
	}
}