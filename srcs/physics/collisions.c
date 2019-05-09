#include "doom_nukem.h"

/*
**	s = perimetre / 2 = somme des trois cote
**	On melange 2 formules sur l'aire du triangle : bh / 2 = sqrt(s(s-a)(s-b)(s-c))
**
**	
*/
/*
static t_dot	direction(t_linedef *line, t_player *player, t_fdot *newpos)
{
	t_dot		dir;

	if (line->equation.a > 0)
	{
		if ()
	}
	else
	{
		if ()
	}
	return (dir);
}*/

static void	gliss(t_linedef *line, t_player *player, t_fdot *newpos)
{
	double	h;
	double	s;
	double	a;
	double	b;
	double	c;
	double	tmp;

	//printf("\nCollisions !!! alpha = %f\n\n", (atan(line->equation.a) * 360) / (2 * M_PI));
	printf("Collisions !!!\n");
	a = dist(line->d2, *newpos);
	b = dist(*newpos, line->d1);
	c = dist(line->d1, line->d2);
	s = (a + b + c) / 2;
	h = (2 * sqrt(s * (s - a) * (s - b) * (s - c))) / c;
	printf("heuteur : %f\n", h);
	//printf("%d\n", line->equation.a * newpos->x + line->equation.b > newpos->y);
	tmp = newpos->x + (line->equation.a * newpos->x + line->equation.b\
			> newpos->y ? 1 : -1) *\
			cos(line->angle) * (h + player->hitbox);
	newpos->y += (line->equation.a * newpos->x + line->equation.b\
				> newpos->y ? 1 : -1) *\
				cos(line->angle) * (h + player->hitbox);
	newpos->x = tmp;
	/*player->pos.x = newpos.x +\
	((newpos.y - line->equation.b) / line->equation.a > newpos.x ? 1 : -1) *\
					sin(atan(line->equation.a)) * (h + player->hitbox);
	player->pos.y = newpos.y +\
	(line->equation.a * newpos.x + line->equation.b > newpos.y ? 1 : -1) *\
					cos(atan(line->equation.a)) * (h + player->hitbox);*/
	printf("New position : %f\t%f\n", player->pos.x, player->pos.y);
}

static void	coef_null(t_linedef *line, t_player *player, t_fdot *newpos)
{
	t_fdot		collision;

	//printf("Coef nulll sa mere\n");
	collision.x = player->pos.x;
	collision.y = line->equation.a * player->pos.x + line->equation.b;
	if (player->vel.y > 0)
	{
		if (player->pos.y < collision.y && collision.y < player->pos.y + player->vel.y)
			gliss(line, player, newpos);
	}
	else if (player->pos.y > collision.y && collision.y > player->pos.y + player->vel.y)
		gliss(line, player, newpos);
}

static void	collisions(t_linedef *line, t_affine traj, t_player *player, t_fdot *newpos)
{
	t_fdot		collision;

	//printf("Move\n");
	//printf("%f\t%f\n", line->equation.a, line->equation.b);
	if (traj.a != line->equation.a)
	{
		collision.x = (line->equation.b - traj.b) / (traj.a - line->equation.a);
		collision.y = traj.a * collision.x + traj.b;
		//printf("\nPosition : %f\t%f\n", player->pos.x, player->pos.y);
		printf("Collision : %f\t%f\n", collision.x, collision.y);
		printf("Dist collision : %f\n", dist(player->pos, collision));
		//printf("Dist vel 2 : %f\n", dist(player->pos, *newpos));
		//printf("Sign 1 : %d\n", sign(player->pos.x - newpos->x));
		//printf("Sign 2 : %d\n", sign(player->pos.x - collision.x));
		/*if (dist(player->pos, collision) > mag(player->vel) || sign(player->pos.x - newpos.x) != sign(player->pos.x - collision.x))
			player->tmppos = newpos;
		else if (!line->portal)
			gliss(line, player, newpos);
		else
			;*/
		if (dist(player->pos, collision) < mag(player->vel) && sign(player->pos.x - newpos->x) == sign(player->pos.x - collision.x))
		{
			if (!line->portal)
				gliss(line, player, newpos);
			else
				;
		}
	}
}

int		move(t_map *map, t_player *player)
{
	int			i;
	t_sector	*sector;
	t_linedef	*line;
	t_fdot		newpos;
	t_affine	traj;

	if (!player->vel.x && !player->vel.y)
		return (0);
	//printf("Move : %f\n%f\n", player->vel.x, player->vel.y);
	newpos = (t_fdot){player->pos.x + player->vel.x,\
						player->pos.y + player->vel.y};
	if (player->vel.x)
		traj.a = (newpos.y - player->pos.y) / (newpos.x - player->pos.x);
	traj.b = player->pos.y - traj.a * player->pos.x;
	//printf("Move 2\n");
	sector = map->sectors;
	i = -1;
	while (++i < player->sector)
		sector = sector->next;
	line = sector->lines;
	while (line)
	{
		//printf("Dist vel 1 : %f\n", dist(player->pos, newpos));
		if (player->vel.x)
			collisions(line, traj, player, &newpos);
		else
			coef_null(line, player, &newpos);
		line = line->next;
	}
	player->pos = newpos;
	printf("\n");
	return (0);
}
