#include "doom_nukem.h"

void	check_file(t_win *win, t_map_editor *map)
{
	t_sector	*sector;
	t_linedef	*line;
	t_linedef	*tmp;

	sector = map->sectors;
	while (sector)
	{
		tmp = NULL;
		line = sector->lines;
		while (line)
		{
			if (line->p1.x == line->p2.x && line->p1.y == line->p2.y)
			{
				if (!tmp)
					sector->lines = line->next;
				else
					tmp->next = line->next;
				ft_strdel(line->name);
				free(line);
			}
			tmp = line;
			line = line->next;
		}
		sector = sector->next;
	}
}
