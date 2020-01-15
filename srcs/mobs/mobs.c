#include "doom_nukem.h"



void		mobs_hit(t_mob *mobs, int damage)
{
	int		dist_min;
	int		tmp;
	t_mob	*closest;

	while (mobs)
	{
		if (closest->alive &&\
			!(mobs->pos.y < -mobs->width / 2 || mobs->width / 2 < mobs->pos.y) &&\
			(tmp = mag(mobs->pos)) < dist_min)
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

void		mobs_attack(t_player *player, t_mob *mobs)
{
	int		dist;

	dist = player->width_2 + mobs->width_2;
	while (mobs)
	{
		if (mobs->alive &&\
			mag(mobs->pos) < dist)
			player->currentHp -= mobs->damage;
		mobs = mobs->next
	}
}

void			mobs_move(t_mob *mobs)
{
	float		cst_vel;

	cst_vel = mag(mobs->pos) / mobs->velvel;
	mobs->pos = (t_fdot_3d){mobs->pos.x - mobs->pos.x / cst_vel,\
							mobs->pos.y - mobs->pos.y / cst_vel,\
							mobs->pos.z - mobs->pos.z / cst_vel};
}
