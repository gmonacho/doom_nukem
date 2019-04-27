#include "physics.h"
#include "data.h"
#include "struct_2d"

/*
**	s = perimetre / 2 = somme des trois cote
**	On melange 2 formules sur l'aire du triangle : bh / 2 = sqrt(s(s-a)(s-b)(s-c))
*/

static void	gliss(t_affine line, t_fdot collision, t_player *player, t_fdot newpos)
{
	double	h;
	double	s;
	double	a;
	double	b;
	double	c;

	a = dist(collision, newpos);
	b = dist(newpos, line.d1);
	c = dist(line.d1, collision)
	s = (a + b + c) / 2;
	h = (2 * sqrt(s * (s - a) * (s - b) * (s - c))) / c;
	player->pos.x = newpos.x + cos(90 - atan(line.a)) * h;
	player->pos.y = newpos.y + cos(-atan(line.a)) * h;
}

static void	collisions(t_affine line, t_affine vel, t_player *player, t_fdot newpos)
{
	t_fdot		collision;

	if (vel->a != line->a)
	{
		collision.x = (line.b - vel.b) / (vel.a - line.a);
		collision.y = vel.a * collision.x + vel.b;
		if (dist(player->pos, collision) + player->hitbox > mag(player->vel))
			player->pos = newpos;
		else
			gliss(line, collision, player, newpos);
	}
}

int		move(t_map *map, t_player *player)
{
	int			i;
	t_sector	*sector;
	t_linedef	*line;
	t_fdot		newpos;
	t_affine	vel;

	if (!(player->vel.x))
		return (0);
	newpos = (t_fdot){player->pos.x + player->vel.x,\
						player->pos.y + player->vel.y};
	vel.a = (newpos.y - player->pos.y) / (newpos.x - player->pos.x);
	vel.b = player->pos.y - vel.a * player->pos.x;
	sector = map->sectors;
	i = -1;
	while (++i < player->sector)
		sector = sector->next;
	line = sector->lines;
	while (line)
	{
		collisions(line->equation, vel, player);
		line = line->next;
	}
	return (0);
}