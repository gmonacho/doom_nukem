#include "doom_nukem.h"

/*
**	Ajouts :
**	- Collisions sur les cotes des la hitbox
**	- gliss sur les murs
**	
*/

static int	actions(t_map *map, t_linedef *portal, t_player *player)
{
	t_sector	*sector;
	t_linedef	*line;

	if (portal->flags & PORTAL)
	{
		sector = map->sectors;
		while (sector)
		{
			line = sector->lines;
			while (line)
			{
				if (portal->id == line->id && portal != line)
					teleportation(portal, line, player);
				line = line->next;
			}
			sector = sector->next;
		}
		return (0);
	}
	else
	{
		player->vel = (t_fvector){0, 0};
		//printf("Rm vitesse\n");
		return (1);
	}
}

static int	coef_null(t_map *map, t_linedef *line, t_player *player)
{
	int		collisiony;

	collisiony = line->equation.a * player->pos.x + line->equation.b;
	//printf("Collision y : %d\n", collisiony);
	if ((line->p1.x <= player->pos.x && player->pos.x <= line->p2.x) ||\
		(line->p2.x <= player->pos.x && player->pos.x <= line->p1.x))
	{
		if (player->vel.y > 0)
		{
			if (player->pos.y < collisiony &&\
				collisiony <= player->pos.y + player->vel.y + player->hitbox)
				return (actions(map, line, player));
		}
		else if (player->pos.y > collisiony &&\
				collisiony >= player->pos.y + player->vel.y - player->hitbox)
			return (actions(map, line, player));
	}
	return (0);
}

static int	collisions(t_map *map, t_linedef *line, t_affine traj,\
						t_player *player)
{
	int		collisionx;
	int		collisiony;

	if (traj.a != line->equation.a)
	{
		if (line->isequation)
			collisionx = (line->equation.b - traj.b) /\
							(traj.a - line->equation.a);
		else
		{
			collisionx = line->equation.a;
			collisiony = traj.a * collisionx + traj.b;
		}
		if ((!(line->isequation) &&\
			((line->p1.y <= collisiony && collisiony <= line->p2.y) ||\
			(line->p2.y <= collisiony && collisiony <= line->p1.y))) ||\
			(line->isequation &&\
			((line->p1.x <= collisionx && collisionx <= line->p2.x) ||\
			(line->p2.x <= collisionx && collisionx <= line->p1.x))))
		{
			//printf("Collision x : %d\n", collisionx);
			if (player->vel.x > 0)
			{
				if (player->pos.x < collisionx &&\
				collisionx <= player->pos.x + player->vel.x + player->hitbox)
					return (actions(map, line, player));
			}
			else if (player->pos.x > collisionx &&\
				collisionx >= player->pos.x + player->vel.x - player->hitbox)
				return (actions(map, line, player));
		}
	}
	return (0);
}

int		move(t_map *map, t_player *player)
{
	int			i;
	t_sector	*sector;
	t_linedef	*line;
	t_affine	traj;

	//printf("\nPos : %f\t%f\n", player->pos.x, player->pos.y);
	//printf("Vel : %f\t%f\n", player->vel.x, player->vel.y);
	if (!player->vel.x && !player->vel.y)
		return (0);
	i = -1;
	sector = map->sectors;
	while (++i < player->sector && sector)
		sector = sector->next;
	if (!sector)
		return (1);
	line = sector->lines;

	//printf("\nVitesse x : %e\n", player->vel.x);
	if ((int)(player->vel.x * 100))
	{
		//printf("Vitesse x != 0 : %f\n", player->vel.x);
		traj.a = player->vel.y / player->vel.x;
		traj.b = player->pos.y - traj.a * player->pos.x;
		while (line)
		{
			if (!(line->flags & DOOR_OPEN) &&\
				collisions(map, line, traj, player))
				break ;
			line = line->next;
		}
	}
	else
	{
		//printf("Vitesse x == 0 : %f\n", player->vel.x);
		while (line)
		{
			if (!(line->flags & DOOR_OPEN) && coef_null(map, line, player))
				break ;
			line = line->next;
		}
	}
	player->pos = (t_fdot){player->pos.x + player->vel.x,\
							player->pos.y + player->vel.y};
	return (0);
}









































SDL_bool	is_in_rect(SDL_Rect rect, t_dot p)
{
	int		tmp;
	
	if (rect.w < 0)
	{
		tmp = rect.x;
		rect.x = rect.x + rect.w;
		rect.w = tmp - rect.x;
	}
	if (rect.h < 0)
	{
		tmp = rect.y;
		rect.y = rect.y + rect.h;
		rect.h = tmp - rect.y;
	}
	return (p.x > rect.x &&
			p.x < rect.x + rect.w &&
			p.y > rect.y &&
			p.y < rect.y + rect.h);
}

SDL_bool	intersect_line_rect(t_line line, SDL_Rect rect)
{
	int		tmp;
	if (rect.w < 0)
	{
		tmp = rect.x;
		rect.x = rect.x + rect.w;
		rect.w = tmp - rect.x;
	}
	if (rect.h < 0)
	{
		tmp = rect.y;
		rect.y = rect.y + rect.h;
		rect.h = tmp - rect.y;
	}
	return (SDL_IntersectRectAndLine(&rect, &line.p1.x, &line.p1.y, &line.p2.x, &line.p2.y));
}

SDL_bool	is_next_point(t_dot dot, t_dot other, int distance)
{
	t_dot	vector;
	int		vnorme;

	vector.x = dot.x - other.x;
	vector.y = dot.y - other.y;
	vnorme = sqrt(vector.x * vector.x + vector.y * vector.y);
	return (vnorme <= distance);
}