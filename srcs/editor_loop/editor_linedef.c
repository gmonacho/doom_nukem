#include "doom_nukem.h"

static int		distance(t_dot p1, t_dot p2)
{
	t_dot	v;

	v.x = abs(p2.x - p1.x);
	v.y = abs(p2.y - p1.y);
	return (sqrt(v.x * v.x + v.y * v.y));
}

SDL_bool 		is_next_to_linedef(t_map_editor *map, t_dot *dot, int radius)
{
	SDL_bool	is_next;
	int			dist;
	t_sector	*s;
	t_linedef 	*l;
	t_dot		target;

	dist = -1;
	is_next = SDL_FALSE;
	s = map->sectors;
	if (dot)
	{
		while (s)
		{
			l = s->lines;
			while (l)
			{
				if (!((dot->x == l->p1.x && dot->y == l->p1.y) || (dot->x == l->p2.x && dot->y == l->p2.y)))
				{
					if (is_next_point(l->p1, *dot, radius))
					{
						if (dist == -1 ||  distance(*dot, l->p1) < dist)
						{
							dist = distance(*dot, l->p1);
							target = l->p1;
						}
						is_next = SDL_TRUE;
					}
					if (is_next_point(l->p2, *dot, radius))
					{
						if (dist == -1 ||  distance(*dot, l->p2) < dist)
						{
							dist = distance(*dot, l->p2);
							target = l->p2;
						}
						is_next = SDL_TRUE;
					}
				}
				l = l->next;
			}
			s = s->next;
		}
	}
	if (is_next)
		*dot = target;
	return (is_next);
}

void	fill_abscissa_ordinate(t_map_editor *map, t_dot mouse)
{
	t_sector	*s;
	t_linedef	*l;
	SDL_bool	a_filled;
	SDL_bool	o_filled;

	s = map->sectors;
	a_filled = SDL_FALSE;
	o_filled = SDL_FALSE;
	while (s)
	{
		l = s->lines;
		while (l)
		{
			if (mouse.x == l->p1.x)
			{
				map->abscissa = (t_line){mouse, l->p1};
				a_filled = SDL_TRUE;
			}
			if ( mouse.y == l->p1.y)
			{
				map->ordinate = (t_line){mouse, l->p1};
				o_filled = SDL_TRUE;
			}
			if (mouse.x == l->p2.x)
			{
				map->abscissa = (t_line){mouse, l->p2};
				a_filled = SDL_TRUE;
			}
			if (mouse.y == l->p2.y)
			{
				map->ordinate = (t_line){mouse, l->p2};
				o_filled = SDL_TRUE;
			}
			l = l->next;
		}
		s = s->next;
	}
	if (!a_filled)
		map->abscissa = (t_line){(t_dot){0, 0}, (t_dot){0, 0}};
	if (!o_filled)
		map->ordinate = (t_line){(t_dot){0, 0}, (t_dot){0, 0}};
}