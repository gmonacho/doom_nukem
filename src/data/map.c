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
	t_sector	*s;
	t_linedef	*l;

	l = map->lines;
	while (l)
	{
		if (intersect_line_rect((t_line){(t_dot){l->p1.x * map->unit + map->x,
												 l->p1.y * map->unit + map->y},
										 (t_dot){l->p2.x * map->unit + map->x,
										 		 l->p2.y * map->unit + map->y}}, map->rect_util))
			l->flags = l->flags | flags;
		else
			l->flags = LINEDEF_NONE;
		l = l->next;
	}
	s = map->sectors;
	while (s)
	{
		l = s->lines;
		while (l)
		{
			if (intersect_line_rect((t_line){(t_dot){l->p1.x * map->unit + map->x,
												 	l->p1.y * map->unit + map->y},
										 	(t_dot){l->p2.x * map->unit + map->x,
										 		 	l->p2.y * map->unit + map->y}}, map->rect_util))
			l->flags = l->flags | flags;
			else
				l->flags = LINEDEF_NONE;
			l = l->next;
		}
		s = s->next;
	}
}

void	delete_linedef(t_map *map, Uint32 delete_flags)
{
	t_sector	*s;
	t_linedef	*l;
	t_linedef	*previous;

	previous = NULL;
	l = map->lines;
	if (l)
	{
		if (l->flags & delete_flags)
		{
			map->lines = map->lines->next;
			free(l);
			l = map->lines;
		}
	}
	while (l)
	{
		if (l->flags & delete_flags)
		{
			if (previous)
			{
				previous->next = l->next;
				free(l);
				l = previous;
			}
		}
		previous = l;
		l = l->next;
	}
	previous = NULL;
	s = map->sectors;
	while (s)
	{
		l = s->lines;
		if (l)
		{
			if (l->flags & delete_flags)
			{
				s->lines = s->lines->next;
				free(l);
				l = s->lines;
			}
		}
		while (l)
		{
			if (l->flags & delete_flags)
			{
				if (previous)
				{
					previous->next = l->next;
					free(l);
					l = previous;
				}
			}
			previous = l;
			l = l->next;
		}
		s = s->next;
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

SDL_bool 		is_next_to_linedef(t_map *map, t_dot *dot, int radius)
{
	t_sector	*s;
	t_linedef 	*l;

	s = map->sectors;
	l = map->lines;
	if (dot)
	{
		while (l)
		{
			if (!((dot->x == l->p1.x && dot->y == l->p1.y) || (dot->x == l->p2.x && dot->y == l->p2.y)))
			{
				if (is_next_point(l->p1, *dot, radius))
				{
					*dot = l->p1;
					return (SDL_TRUE);
				}
				if (is_next_point(l->p2, *dot, radius))
				{
					*dot = l->p2;
					return (SDL_TRUE);
				}
			}
			l = l->next;
		}
		while (s)
		{
			l = s->lines;
			while (l)
			{
				if (!((dot->x == l->p1.x && dot->y == l->p1.y) || (dot->x == l->p2.x && dot->y == l->p2.y)))
				{
					if (is_next_point(l->p1, *dot, radius))
					{
						*dot = l->p1;
						return (SDL_TRUE);
					}
					if (is_next_point(l->p2, *dot, radius))
					{
						*dot = l->p2;
						return (SDL_TRUE);
					}
				}
				l = l->next;
			}
			s = s->next;
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
	sector->name = "Unnamed";
	sector->color = (SDL_Color){255, 0, 0, 255};
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

int			get_nb_sectors(t_sector *sector)
{
	t_sector	*s;
	int			i;

	s = sector;
	i = 0;
	while (s)
	{
		i++;
		s = s->next;
	}
	return (i);
}

void		map_zoom(t_map *map, double zoom)
{
	map->x -= map->unit * zoom * map->w / 2;
	map->y -= map->unit * zoom * map->h / 2;
	map->unit += map->unit * zoom;
}
