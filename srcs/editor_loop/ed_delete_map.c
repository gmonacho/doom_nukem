#include "doom_nukem.h"

void	ed_delete_map(void *map_ptr)
{
	t_map	*map;
	t_poly	*p;
	t_poly	*next;

	map = (t_map*)map_ptr;
	if (map)
	{
		p = map->polys;
		while (p)
		{
			next = p->next;
			free(p);
			p = next;
		}
		map->polys = NULL;
	}
}