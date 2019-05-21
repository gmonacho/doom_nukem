#include "doom_nukem.h"

static void	coef_null(t_linedef *line, t_player *player)
{
	int		collisiony;

	collisiony = line->equation.a * player->pos.x + line->equation.b;
	if ((line->p1.x < player->pos.x && player->pos.x < line->p2.x) ||\
		(line->p2.x < player->pos.x && player->pos.x < line->p1.x))
	{
		if (player->vel.y > 0)
		{
			if (player->pos.y < collisiony &&\
				collisiony <= player->pos.y + player->vel.y + player->hitbox)
				player->vel = (t_vector){};
		}
		else if (player->pos.y > collisiony &&\
		collisiony >= player->pos.y + player->vel.y - player->hitbox)
			player->vel = (t_vector){};
	}
}

static void	collisions(t_linedef *line, t_affine traj,\
						t_player *player)
{
	int		collisionx;

	if (traj.a != line->equation.a)
	{
		if (line->isequation)
			collisionx = (line->equation.b - traj.b) / (traj.a - line->equation.a);
		else
			collisionx = line->equation.a;

		if ((line->p1.x < collisionx && collisionx < line->p2.x) ||\
			(line->p2.x < collisionx && collisionx < line->p1.x))
		{
			if (player->vel.x > 0)
			{
				if (player->pos.x < collisionx && collisionx <= player->pos.x + player->vel.x + player->hitbox)
					player->vel = (t_vector){};
			}
			else if (player->pos.x > collisionx && collisionx >= player->pos.x + player->vel.x - player->hitbox)
				player->vel = (t_vector){};
		}
	}
}

int		move(t_map *map, t_player *player)
{
	int			i;
	t_sector	*sector;
	t_linedef	*line;
	t_affine	traj;

	if (!player->vel.x && !player->vel.y)
		return (0);
	i = -1;
	sector = map->sectors;
	while (++i < player->sector)
		sector = sector->next;
	line = sector->lines;
	if (player->vel.x)
	{
		traj.a = player->vel.y / player->vel.x;
		traj.b = player->pos.y - traj.a * player->pos.x;
		while (line)
		{
			collisions(line, traj, player);
			line = line->next;
		}
	}
	else
		while (line)
		{
			coef_null(line, player);
			line = line->next;
		}
	player->pos = (t_dot){player->pos.x + player->vel.x, player->pos.y + player->vel.y};
	printf("\n");
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

SDL_bool intersect_line_rect(t_line line, SDL_Rect rect)
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

SDL_bool is_next_point(t_dot dot, t_dot other, int distance)
{
	t_dot	vector;
	int		vnorme;

	vector.x = dot.x - other.x;
	vector.y = dot.y - other.y;
	vnorme = sqrt(vector.x * vector.x + vector.y * vector.y);
	return (vnorme <= distance);
}