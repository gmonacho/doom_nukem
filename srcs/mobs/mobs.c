#include "doom_nukem.h"

// void		init_mobs(t_mob *mobs, t_poly *poly)
// {

// }

void			mobs_hit(t_mob *mobs, int damage)
{
	int			dist_min;
	int			tmp;
	t_mob		*closest;

	dist_min = -1;
	while (mobs)
	{
		if (mobs->alive &&\
			!(mobs->pos.y < -mobs->width_2 || mobs->width_2 < mobs->pos.y) &&\
			(dist_min == -1 || (tmp = mag(mobs->pos)) < dist_min))
		{
			dist_min = tmp;
			closest = mobs;
		}
		mobs = mobs->next;
	}
	if (closest)
	{
		closest->health -= damage;
		if (closest->health <= 0)
			closest->alive = 0;
	}
}

static void		mobs_move(t_fdot_3d pos, t_poly *poly, float vel)
{
	t_fdot_3d	move;
	float		cst_vel;

	cst_vel = vel / mag(pos);
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
	// printf("cst_vel %f\n", cst_vel);
	// printf("Pos mob %f %f %f\n", mob->pos.x, mob->pos.y, mob->pos.z);
}

void			mobs_attack_move(t_player *player, t_mob *mobs)
{
	int			dist;
	t_fdot_3d	pos;

	dist = player->width_2 + mobs->width_2;
	while (mobs)
	{
		pos = (t_fdot_3d){(mobs->poly->dots_rotz_only[0].x + mobs->poly->dots_rotz_only[2].x) / 2,\
							(mobs->poly->dots_rotz_only[0].y + mobs->poly->dots_rotz_only[2].y) / 2,\
							(mobs->poly->dots_rotz_only[0].z + mobs->poly->dots_rotz_only[2].z) / 2};
		if (mobs->alive)
		{
			// printf("Dist mobs player %f\n", mag(pos));
			if (mag(pos) < dist)
				apply_damage(player, mobs->damage);
			else
				mobs_move(pos, mobs->poly, mobs->vel);
		}
		mobs = mobs->next;
	}
}
