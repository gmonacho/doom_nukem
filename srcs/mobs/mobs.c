#include "doom_nukem.h"

// void		init_mobs(t_mob *mobs, t_poly *poly)
// {

// }

void		mobs_hit(t_mob *mobs, int damage)
{
	int		dist_min;
	int		tmp;
	t_mob	*closest;

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

void		mobs_attack_move(t_player *player, t_mob *mobs)
{
	float	cst_vel;
	int		dist;

	dist = player->width_2 + mobs->width_2;
	while (mobs)
	{
		if (mobs->alive)
		{
			if (mag(mobs->pos) < dist)
				player->currentHp -= mobs->damage;
			else
			{
				cst_vel = mobs->vel / mag(mobs->pos);
				mobs->pos = (t_fdot_3d){mobs->pos.x - mobs->pos.x * cst_vel,\
										mobs->pos.y - mobs->pos.y * cst_vel,\
										mobs->pos.z - mobs->pos.z * cst_vel};
				set_mobs_dots(mobs);
				printf("cst_vel %f\n", cst_vel);
			}
		}
		mobs = mobs->next;
	}
}

