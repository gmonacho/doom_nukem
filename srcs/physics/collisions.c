#include "doom_nukem.h"

/*
**	s = perimetre / 2 = somme des trois cote
**	On melange 2 formules sur l'aire du triangle : bh / 2 = sqrt(s(s-a)(s-b)(s-c))
*/

static void	gliss(t_linedef line, t_fdot collision, t_player *player, t_fdot newpos)
{
	double	h;
	double	s;
	double	a;
	double	b;
	double	c;

	a = dist(collision, newpos);
	b = dist(newpos, line.d1);
	c = dist(line.d1, collision);
	s = (a + b + c) / 2;
	h = (2 * sqrt(s * (s - a) * (s - b) * (s - c))) / c;
	player->pos.x = newpos.x + cos(90 - atan(line.equation.a)) * h;
	player->pos.y = newpos.y + cos(-atan(line.equation.a)) * h;
}

static void	collisions(t_linedef *line, t_affine vel, t_player *player, t_fdot newpos)
{
	t_fdot		collision;

	if (vel.a != line->equation.a)
	{
		collision.x = (line->equation.b - vel.b) / (vel.a - line->equation.a);
		collision.y = vel.a * collision.x + vel.b;
		if (dist(player->pos, collision) + player->hitbox > mag(player->vel))
			player->pos = newpos;
		else if (!line->portal)
			gliss(*line, collision, player, newpos);
		else
			;/*Changement de secteur*/
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
		collisions(line, vel, player);
		line = line->next;
	}
	return (0);
}