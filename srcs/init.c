#include "doom_nukem.h"

static int		find_portal_id(t_map *map, t_linedef *line1, int id)
{
	t_sector	*sector;
	t_linedef	*line;

	sector = map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			if (id == line->id && line1 != line)
			{
				line1->destline = line;
				return (0);
			}
			line = line->next;
		}
		sector = sector->next;
	}
	return (1);
}

int		init_lines(t_map *map)
{
	t_sector	*sector;
	t_linedef	*line;

	sector = map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			line->sector = sector;
			if (line->flags & PORTAL && find_portal_id(map, line, line->id))
				return (1);
			line = line->next;
		}
		sector = sector->next;
	}
	return (0);
}

static void		set_center(t_map *map)
{
	t_sector	*sector;
	t_linedef	*line;
	int			ndot;

	ndot = 0;
	sector = map->sectors;
	while (sector)
	{
		ndot = 0;
		line = sector->lines;
		while (line)
		{
			sector->center.x += line->p1.x;
			sector->center.y += line->p1.y;
			ndot++;
			line = line->next;
		}
		if (ndot)
		{
			sector->center.x /= ndot;
			sector->center.y /= ndot;
		}
		printf("Center : %d\t%d\tndot : %d\n", sector->center.x, sector->center.y, ndot);
		sector = sector->next;
	}
}

int		init_sectors(t_map *map, t_player *player)
{
	t_sector	*sector;
	int			i;

	set_center(map);
	sector = map->sectors;
	while (sector)
	{
		if (sector->floor_height >= sector->ceil_height)
			return (1);
		sector->height = sector->ceil_height - sector->floor_height;
		sector = sector->next;
	}
	player->sector = map->sectors;
	i = -1;
	while (++i < player->numsector)
		player->sector = player->sector->next;
	/*if (player->height > player->sector->height)
		return (1);*/
	printf("Ad : %p\n", map->sectors);
	printf("Ad : %p\n\n", map->player.sector);
	return (0);
}