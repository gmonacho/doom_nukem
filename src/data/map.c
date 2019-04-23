#include "display.h"
#include "struct_2d.h"
#include "libft.h"
#include "ret_error.h"
#include "data.h"

int		map_add_line(t_map *map, t_linedef line)
{
	t_linedef	*tmp;
	int			i;
	
	tmp = map->lines;
	if (!(map->lines = (t_linedef*)ft_memalloc(sizeof(t_linedef) * (map->nb_lines + 1))))
		return (ret_perror("lines allocation failed in map_add_line"));
	i = 0;
	while (i < map->nb_lines)
	{
		map->lines[i] = tmp[i];
		i++;
	}
	map->lines[map->nb_lines] = line;
	map->nb_lines++;
	free(tmp);
	return (1);
}

t_linedef	new_linedef(t_line line, SDL_Texture *p1p2, SDL_Texture *p2p1, Uint32 flags)
{
	t_linedef linedef;

	linedef.p1 = line.p1;
	linedef.p2 = line.p2;
	linedef.p1p2_texture = p1p2;
	linedef.p2p1_texture = p2p1;
	linedef.flags = flags;
	return (linedef);
}

void		map_zoom(t_win *win, t_map *map, double zoom)
{
	map->x -= ((map->w * map->unit) - map->w * map->unit * zoom) / 2;
	map->y -= ((map->h * map->unit) - map->h * map->unit * zoom) / 2;
	if (win)
		map->unit *= zoom;
}