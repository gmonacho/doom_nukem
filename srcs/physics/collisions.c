#include "doom_nukem.h"

/*
**	s = perimetre / 2 = somme des trois cote
**	On melange 2 formules sur l'aire du triangle : bh / 2 = sqrt(s(s-a)(s-b)(s-c))
*/

static void	gliss(t_linedef *line, t_player *player, t_fdot newpos)
{
	double	h;
	double	s;
	double	a;
	double	b;
	double	c;

	//printf("\nCollisions !!! alpha = %f\n\n", (atan(line->equation.a) * 360) / (2 * M_PI));
	printf("Collisions !!!\n");
	a = dist(line->d2, newpos);
	b = dist(newpos, line->d1);
	c = dist(line->d1, line->d2);
	s = (a + b + c) / 2;
	h = (2 * sqrt(s * (s - a) * (s - b) * (s - c))) / c;
	player->pos.x = newpos.x +\
	((newpos.y - line->equation.b) / line->equation.a > newpos.x ? 1 : -1) *\
					sin(atan(line->equation.a)) * (h + player->hitbox);
	player->pos.y = newpos.y +\
	(line->equation.a * newpos.x + line->equation.b > newpos.y ? 1 : -1) *\
					cos(atan(line->equation.a)) * (h + player->hitbox);
	printf("New position : %f\t%f\n", player->pos.x, player->pos.y);
}

static void	coef_null(t_linedef *line, t_player *player, t_fdot newpos)
{
	t_fdot		collision;

	//printf("Coef nulll sa mere\n");
	collision.x = player->pos.x;
	collision.y = line->equation.a * player->pos.x + line->equation.b;
	if (player->vel.y > 0)
	{
		if (player->pos.y < collision.y && collision.y < player->pos.y + player->vel.y)
			gliss(line, player, newpos);
		else
			player->pos = newpos;
	}
	else
	{
		if (player->pos.y > collision.y && collision.y > player->pos.y + player->vel.y)
			gliss(line, player, newpos);
		else
			player->pos = newpos;
	}
}

static void	collisions(t_linedef *line, t_affine vel, t_player *player, t_fdot newpos)
{
	t_fdot		collision;

	//printf("Move\n");
	//printf("%f\t%f\n", line->equation.a, line->equation.b);
	if (vel.a == line->equation.a)
		player->pos = newpos;
	else
	{
		collision.x = (line->equation.b - vel.b) / (vel.a - line->equation.a);
		collision.y = vel.a * collision.x + vel.b;
		printf("Collision : %f\t%f\n", collision.x, collision.y);
		printf("Dist collision : %f\n", dist(player->pos, collision));
		if (dist(player->pos, collision) > mag(player->vel)/* + player->hitbox*/)
			player->pos = newpos;
		else if (!line->portal)
			gliss(line, player, newpos);
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

	printf("\nPosition : %f\t%f\n", player->pos.x, player->pos.y);
	if (!player->vel.x && !player->vel.y)
		return (0);
	//printf("Move : %f\n%f\n", player->vel.x, player->vel.y);
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
	printf("\n");
	return (0);
}