#include "display.h"
#include "libft.h"
#include "ret_error.h"
#include "data.h"
#include "physics.h"
#include "struct_2d.h"

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

SDL_bool	is_line_horizontal(int y1, int y2, int pitch)
{
	return (abs(y1 - y2) < pitch);
}

SDL_bool	is_line_vertical(int x1, int x2, int pitch)
{
	return (abs(x1 - x2) < pitch);
}

void		map_zoom(t_map *map, double zoom)
{
	map->x -= map->unit * zoom * map->w / 2;
	map->y -= map->unit * zoom * map->h / 2;
	map->unit += map->unit * zoom;
}
