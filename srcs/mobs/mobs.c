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

static void		mobs_move(t_mob *mob, t_poly *poly)
{
	t_fdot_3d	move;
	float		cst_vel;

	cst_vel = mob->vel / mag(mob->pos);
	move = (t_fdot_3d){-mob->pos.x * cst_vel,\
						-mob->pos.y * cst_vel,\
						-mob->pos.z * cst_vel};
	translate_dot(&(mob->pos), move);
	translate_dot(&(poly->dots_rotz_only[0]), move);
	translate_dot(&(poly->dots_rotz_only[1]), move);
	translate_dot(&(poly->dots_rotz_only[2]), move);
	translate_dot(&(poly->dots_rotz_only[3]), move);
	poly->equation_rotz_only.d = -(poly->equation_rotz_only.v.x * mob->pos.x +\
									poly->equation_rotz_only.v.y * mob->pos.y +\
									poly->equation_rotz_only.v.z * mob->pos.z);
	// printf("cst_vel %f\n", cst_vel);
	// printf("Pos mob %f %f %f\n", mob->pos.x, mob->pos.y, mob->pos.z);
}

void			mobs_attack_move(t_player *player, t_mob *mobs)
{
	int			dist;

	dist = player->width_2 + mobs->width_2;
	while (mobs)
	{
		if (mobs->alive)
		{
			if (mag(mobs->pos) < dist)
				player->currentHp -= mobs->damage;
			else
				mobs_move(mobs, mobs->poly);
		}
		mobs = mobs->next;
	}
}

