#include "doom_nukem.h"

void	map_add_line(t_map *map, int n_sector, t_linedef *line)
{
	t_sector	*sector;
	t_linedef	*tmp;
	int			i;

	sector = map->sectors;
	i = -1;
	while (++i < n_sector)
		sector = sector->next;
	if (sector->lines)
	{
		tmp = sector->lines;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = line;
	}
	else
		sector->lines = line;
}

t_linedef	*new_linedef(t_line line, SDL_Texture *texture, Uint32 flags)
{
	t_linedef	*newline;

	if (!(newline = (t_linedef *)ft_memalloc(sizeof(t_linedef))))
		return (ret_null_perror("lines allocation failed in map_add_line"));
	newline->d1 = line.d1;
	newline->d2 = line.d2;
	newline->equation.a = (line.d2.y - line.d1.y) /\
						(line.d2.x - line.d1.x);
	newline->equation.b = line.d1.y - newline->equation.a * line.d1.x;
	newline->portal = 0;
	newline->p1p2_texture = texture;
	newline->flags = flags;
	newline->next = NULL;
	return (newline);
}

/*void		map_zoom(t_win *win, t_map *map)
{
	win->x += zoom;
	win->y += zoom;
	if (win)
		map->unit *= zoom;
}*/
