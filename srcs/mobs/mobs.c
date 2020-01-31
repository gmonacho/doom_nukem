#include "doom_nukem.h"

void				mobs_hit(t_mob *mobs, int damage)
{
	t_cartesienne	ray;
	t_mob			*closest;
	t_fdot_3d		collision;
	t_fdot			coord_plan;
	int				tmp;
	int				dist_min;

	dist_min = -1;
	ray = (t_cartesienne){0, 0, 0, 1, 0, 0, 0, NULL, 0, NULL};
	while (mobs)
	{
		if (mobs->alive)
		{
			if (!intersection_plan_my_ray(&collision, mobs->poly->equation, &ray))
				continue ;
			printf("Collision %f %f %f\n", collision.x, collision.y, collision.z);
			if ((dist_min == -1 || (tmp = mag(collision)) < dist_min) &&\
				is_in_poly(mobs->poly, &coord_plan, collision))
			{
				dist_min = tmp;
				closest = mobs;
			}
		}
		mobs = mobs->next;
	}
	if (closest)
	{
		closest->health -= damage;
		printf("Hit %d\n", closest->health);
		if (closest->health <= 0)
			closest->alive = 0;
	}
}

// void			mobs_hit(t_mob *mobs, int damage)
// {
// 	int			dist_min;
// 	int			tmp;
// 	t_mob		*closest;

// 	dist_min = -1;
// 	while (mobs)
// 	{
// 		if (mobs->alive &&\
// 			!(mobs->pos.y < -mobs->width_2 || mobs->width_2 < mobs->pos.y) &&\
// 			(dist_min == -1 || (tmp = mag(mobs->pos)) < dist_min))
// 		{
// 			dist_min = tmp;
// 			closest = mobs;
// 		}
// 		mobs = mobs->next;
// 	}
// 	if (closest)
// 	{
// 		closest->health -= damage;
// 		if (closest->health <= 0)
// 			closest->alive = 0;
// 		printf("Mob hit -%d -> %d\n", damage, closest->health);
// 	}
// }

static void		mobs_move(t_fdot_3d pos, t_poly *poly, float dist_mob, float vel)
{
	t_fdot_3d	move;
	float		cst_vel;

	cst_vel = vel / dist_mob;
	move = (t_fdot_3d){-pos.x * cst_vel,\
						-pos.y * cst_vel,\
						-pos.z * cst_vel};
	translate_dot(&(pos), move);
	translate_dot(&(poly->dots_rotz_only[0]), move);
	translate_dot(&(poly->dots_rotz_only[1]), move);
	translate_dot(&(poly->dots_rotz_only[2]), move);
	translate_dot(&(poly->dots_rotz_only[3]), move);
	poly->equation_rotz_only.d = -(poly->equation_rotz_only.v.x * poly->dots_rotz_only[0].x +\
									poly->equation_rotz_only.v.y * poly->dots_rotz_only[0].y +\
									poly->equation_rotz_only.v.z * poly->dots_rotz_only[0].z);
	// printf("cst_vel %f\tvel %f\tdits_mob %f\n", cst_vel, vel, dist_mob);
}

void			mobs_attack_move(t_player *player, t_mob *mobs)
{
	t_fdot_3d	pos;
	float		dist_mob;
	float		dist;

	dist = player->width_2 + mobs->width_2;
	while (mobs)
	{
		if (mobs->alive)
		{
			pos = (t_fdot_3d){(mobs->poly->dots_rotz_only[0].x + mobs->poly->dots_rotz_only[2].x) / 2,\
								(mobs->poly->dots_rotz_only[0].y + mobs->poly->dots_rotz_only[2].y) / 2,\
								(mobs->poly->dots_rotz_only[0].z + mobs->poly->dots_rotz_only[2].z) / 2};
			if (is_null(dist_mob = mag(pos), 1) || dist_mob < dist)
				apply_damage(player, mobs->damage);
			else
				mobs_move(pos, mobs->poly, dist_mob, mobs->vel);
		}
		mobs = mobs->next;
	}
}
