#include "doom_nukem.h"

t_dot	ed_is_next_to_poly(const t_map *map, t_dot point, int radius)
{
	t_poly	*p;
	int		i;

	p = map->polys;
	while (p)
	{
		i = 0;
		while (i < 4)
		{
			if (ed_get_line_len(&(t_line){point, (t_dot){p->dots[i].x, p->dots[i].y}}) <= radius)
				return ((t_dot){p->dots[i].x, p->dots[i].y});
			i++;
		}
		p = p->next;
	}
	return (point);
}
