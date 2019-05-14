#include "doom_nukem.h"

void		delete_linedef(t_linedef **lines, Uint32 delete_flags)
{
	t_linedef	*new_lines;
	t_linedef	*tmp;

	new_lines = NULL;
	while ((*lines))
	{
		if (!((*lines)->flags & delete_flags))
		{
			add_linedef(&new_lines, (*lines));
			(*lines) = (*lines)->next;
		}
		else
		{
			tmp = (*lines);
			(*lines) = (*lines)->next;
			free(tmp);
		}
	}
	*lines = new_lines;
}

void		delete_sector_linedef(t_sector *sectors, Uint32 delete_flags)
{
	t_sector	*s;

	s = sectors;
	while (s)
	{
		delete_linedef(&s->lines, delete_flags);
		s = s->next;
	}
}

void		delete_all_linedef(t_map_editor *map, Uint32 delete_flags)
{
	delete_linedef(&map->lines, delete_flags);
	delete_sector_linedef(map->sectors, delete_flags);
}