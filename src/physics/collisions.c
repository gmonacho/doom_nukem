#include "physics.h"
#include "data.h"
#include "struct_2d"

void	intersection(t_affine line, t_affine vel, t_player *player, t_fdot newpos)
{
	t_fdot		collision;

	if (vel->a != line->a)
	{
		collision.x = (line.b - vel.b) / (vel.a - line.a);
		collision.y = vel.a * collision.x + vel.b;
		if (dist(player->pos, collision) > mag(player->vel))
			player->pos = newpos;
	}
}

int		collisions(t_map *map, t_player *player)
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
		intersection(line->equation, vel, player);
		line = line->next;
	}
	return (0);
}