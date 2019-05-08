#include "display.h"
#include "libft.h"
#include "ret_error.h"
#include "data.h"
#include "physics.h"
#include "struct_2d.h"

void		add_linedef(t_linedef **lines, t_linedef *new_linedef)
{
	new_linedef->next = *lines;
	*lines = new_linedef;
}

t_linedef	*new_linedef(t_line line, SDL_Texture *p1p2, SDL_Texture *p2p1, Uint32 flags)
{
	t_linedef *linedef;

	if (!(linedef = (t_linedef*)ft_memalloc(sizeof(t_linedef))))
		return (ret_null_perror("linedef allocation failed in new_linedef"));
	linedef->p1 = line.p1;
	linedef->p2 = line.p2;
	linedef->p1p2_texture = p1p2;
	linedef->p2p1_texture = p2p1;
	linedef->flags = flags;
	linedef->next = NULL;
	return (linedef);
}

void	selected_linedef(t_map *map, Uint32 flags)
{
	t_linedef	*tmp;

	tmp = map->lines;
	while (tmp)
	{
		if (intersect_line_rect((t_line){(t_dot){tmp->p1.x * map->unit + map->x,
												 tmp->p1.y * map->unit + map->y},
										 (t_dot){tmp->p2.x * map->unit + map->x,
										 		 tmp->p2.y * map->unit + map->y}}, map->rect_util))
			tmp->flags = tmp->flags | flags;
		else
			tmp->flags = LINEDEF_NONE;
		tmp = tmp->next;
	}
}

void	delete_linedef(t_linedef **lines, Uint32 delete_flags)
{
	t_linedef	*tmp;
	t_linedef	*previous;

	previous = NULL;
	tmp = *lines;
	if (tmp)
	{
		if (tmp->flags & delete_flags)
		{
			(*lines) = (*lines)->next;
			free(tmp);
			tmp = *lines;
		}
	}
	while (tmp)
	{
		if (tmp->flags & delete_flags)
		{
			if (previous)
			{
				previous->next = tmp->next;
				free(tmp);
				tmp = previous;
			}
		}
		previous = tmp;
		tmp = tmp->next;
	}
}

int			map_get_nb_linedef(t_linedef *lines)
{
	t_linedef 	*tmp;
	int			nb;

	nb = 0;
	tmp = lines;
	while (tmp)
	{
		nb++;
		tmp = tmp->next;
	}
	return (nb);
}

SDL_bool 		is_next_to_linedef(t_linedef **lines, t_dot *dot, int radius)
{
	t_linedef 	*tmp;

	tmp = *lines;
	if (dot)
	{
		while (tmp)
		{
			if (!((dot->x == tmp->p1.x && dot->y == tmp->p1.y) || (dot->x == tmp->p2.x && dot->y == tmp->p2.y)))
			{
				if (is_next_point(tmp->p1, *dot, radius))
				{
					*dot = tmp->p1;
					return (SDL_TRUE);
				}
				if (is_next_point(tmp->p2, *dot, radius))
				{
					*dot = tmp->p2;
					return (SDL_TRUE);
				}
			}
			tmp = tmp->next;
		}
	}
	return (SDL_FALSE);
}


SDL_bool	is_line_horizontal(int y1, int y2, int pitch)
{
	return (abs(y1 - y2) < pitch);
}

SDL_bool	is_line_vertical(int x1, int x2, int pitch)
{
	return (abs(x1 - x2) < pitch);
}

t_sector	*new_sector()
{
	t_sector *sector;

	if (!(sector = (t_sector*)ft_memalloc(sizeof(t_sector))))
		return (ret_null_perror("sector allocation failed in new_sector"));
	sector->floor_height = 0;
	sector->floor_texture = NULL;
	sector->ceil_height = 0;
	sector->ceil_texture = NULL;
	sector->light_level = 0;
	sector->lines = NULL;
	sector->next = NULL;
	return (sector);
}

void		add_sector(t_sector **sectors, t_sector *new_sector)
{
	new_sector->next = *sectors;
	*sectors = new_sector;
}

void		map_zoom(t_map *map, double zoom)
{
	map->x -= map->unit * zoom * map->w / 2;
	map->y -= map->unit * zoom * map->h / 2;
	map->unit += map->unit * zoom;
}
