#include "doom_nukem.h"

/*
**	s = perimetre / 2 = somme des trois cote
**	On melange 2 formules sur l'aire du triangle : bh / 2 = sqrt(s(s-a)(s-b)(s-c))
*/

static void	gliss(t_linedef *line, t_fdot collision, t_player *player, t_fdot newpos)
{
	double	h;
	double	s;
	double	a;
	double	b;
	double	c;

	printf("Collisions !\n");
	a = dist(collision, newpos);
	b = dist(newpos, line->d1);
	c = dist(line->d1, collision);
	s = (a + b + c) / 2;
	h = (2 * sqrt(s * (s - a) * (s - b) * (s - c))) / c;
	player->pos.x = newpos.x + sin(atan(line->equation.a)) * (h/* + player->hitbox*/);
	player->pos.y = newpos.y + cos(atan(line->equation.a)) * (h/* + player->hitbox*/);
	//player->pos.x = newpos.x + cos(M_PI_4 - atan(line->equation.a)) * h;
	//player->pos.y = newpos.y + cos(-atan(line->equation.a)) * h;
}

static void	coef_null(t_linedef *line, t_player *player, t_fdot newpos)
{
	t_fdot		collision;

	collision.x = player->pos.x;
	collision.y = line->equation.a * player->pos.x + line->equation.b;
	if (player->vel.y > 0)
	{
		if (player->pos.y < collision.y && collision.y < player->pos.y + player->vel.y)
			gliss(line, collision, player, newpos);
		else
			player->pos = newpos;
	}
	else
		if (player->pos.y > collision.y && collision.y > player->pos.y + player->vel.y)
			gliss(line, collision, player, newpos);
		else
			player->pos = newpos;
}

static void	collisions(t_linedef *line, t_affine vel, t_player *player, t_fdot newpos)
{
	t_fdot		collision;

	//printf("Move\n");
	if (vel.a == line->equation.a)
		player->pos = newpos;
	else
	{
		collision.x = (line->equation.b - vel.b) / (vel.a - line->equation.a);
		collision.y = vel.a * collision.x + vel.b;
		if (dist(player->pos, collision) < mag(player->vel) + player->hitbox)
			player->pos = newpos;
		else if (!line->portal)
			gliss(line, collision, player, newpos);
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

	if (!player->vel.x && !player->vel.y)
		return (0);
	//printf("Move\n");
	newpos = (t_fdot){player->pos.x + player->vel.x,\
						player->pos.y + player->vel.y};
	if (player->vel.x)
		vel.a = (newpos.y - player->pos.y) / (newpos.x - player->pos.x);
	vel.b = player->pos.y - vel.a * player->pos.x;
	//printf("Move 2\n");
	sector = map->sectors;
	i = -1;
	while (++i < player->sector)
		sector = sector->next;
	line = sector->lines;
	while (line)
	{
		if (player->vel.x)
			collisions(line, vel, player, newpos);
		else
			coef_null(line, player, newpos);
		line = line->next;
	}
	//printf("Fin Move\n");
	return (0);
}