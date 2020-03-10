/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   mobs_move.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <gmonacho@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/03/05 19:46:19 by agiordan     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/09 16:24:39 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */
#include "doom_nukem.h"

/*
static int			is_in_shadow((t_map *map, t_fdot_3d mob)
{
	t_poly			*poly;
	t_fdot_3d		intersec;
	t_fdot			coord;
	t_cartesienne		ray;

	ray = (t_cartesienne){0, 0, 0,\
				mob.x, mob.y, mob.z,\
				0, NULL, 0, (t_fdot_3d){0, 0, 0}, NULL};
	poly = map->polys;
	while (poly)
	{
		if (!poly->mob && (!poly->object ||\
			(poly->visible == 1 && (poly->object->DOOR ||\
			poly->object->BOX))) &&\
			intersection_plan_ray(&intersec, poly->equation, ray) &&\
			is_in_segment(intersec, (t_fdot_3d){0, 0, 0}, mob) &&\
			is_in_poly(poly, &coord, intersec))
			return (1);
		poly = poly->next;
	}
	return (0);
}
*/

static void			mobs_move(t_fdot_3d pos, t_poly *poly,\
							float dist_mob, float vel)
{
	t_fdot_3d		move;
	float			cst_vel;

	cst_vel = vel / dist_mob;
	move = (t_fdot_3d){-pos.x * cst_vel,\
						-pos.y * cst_vel,\
						-pos.z * cst_vel};
	poly->dots_rotz_only[0] = fdot_3d_add(poly->dots_rotz_only[0], move);
	poly->dots_rotz_only[1] = fdot_3d_add(poly->dots_rotz_only[1], move);
	poly->dots_rotz_only[2] = fdot_3d_add(poly->dots_rotz_only[2], move);
	poly->dots_rotz_only[3] = fdot_3d_add(poly->dots_rotz_only[3], move);
	poly->equation_rotz_only.d = -scalar_product(poly->equation_rotz_only.v,\
													poly->dots_rotz_only[0]);
}

static void			mobs_apply_rotation(t_poly *poly, t_fdot_3d pos,\
										t_matrix rotation)
{
	poly->dots_rotz_only[0] =\
				fdot_3d_add(rotate_dot(fdot_3d_sub(poly->dots_rotz_only[0],\
													pos),\
										rotation),\
							pos);
	poly->dots_rotz_only[1] =\
				fdot_3d_add(rotate_dot(fdot_3d_sub(poly->dots_rotz_only[1],\
													pos),\
										rotation),\
							pos);
	poly->dots_rotz_only[2] =\
				fdot_3d_add(rotate_dot(fdot_3d_sub(poly->dots_rotz_only[2],\
													pos),\
										rotation),\
							pos);
	poly->dots_rotz_only[3] =\
				fdot_3d_add(rotate_dot(fdot_3d_sub(poly->dots_rotz_only[3],\
													pos),\
										rotation),\
							pos);
}

static void			mobs_rotate(t_player *player, t_fdot_3d pos, t_poly *poly)
{
	t_fdot_3d		v1;
	t_fdot_3d		v2;
	t_matrix		rotation;

	v1 = rotate_dot(poly->equation_rotz_only.v, player->rz);
	v2 = rotate_dot(poly->equation_rotz_only.v, player->rz_inv);
	if (mag(fdot_3d_add(pos, v1)) < mag(fdot_3d_add(pos, v2)))
	{
		poly->equation_rotz_only.v = v1;
		rotation = player->rz;
	}
	else
	{
		poly->equation_rotz_only.v = v2;
		rotation = player->rz_inv;
	}
	mobs_apply_rotation(poly, pos, rotation);
}

void				mobs_attack_move(t_map *map, t_player *player, t_mob *mobs)
{
	t_fdot_3d		pos;
	float			dist_mob;
	float			dist;

	if (mobs)
	{
		dist = player->width_2 + mobs->width_2;
		while (mobs)
		{
			if (mobs->alive)
			{
				pos = mid_segment(mobs->poly->dots_rotz_only[0],\
									mobs->poly->dots_rotz_only[2]);
				mobs_rotate(player, pos, mobs->poly);
				if (is_null(dist_mob = mag(pos), 1) || dist_mob < dist)
					apply_damage(map, player, mobs->damage);
				//else if (!is_in_shadow(map, pos))
				else
					mobs_move(pos, mobs->poly, dist_mob, mobs->vel);
			}
			mobs = mobs->next;
		}
	}
}
