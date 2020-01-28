#include "doom_nukem.h"

static SDL_bool	ed_is_wall_selected(const t_map *map, const t_poly *poly)
{
	int		x1;
	int		y1;
	int		x2;
	int		y2;

	x1 = poly->dots[0].x;
	y1 = poly->dots[0].y;
	x2 = poly->dots[1].x;
	y2 = poly->dots[1].y;
	return (SDL_IntersectRectAndLine((SDL_Rect*)&map->editor.select_rect,
			&x1, &y1, &x2, &y2));
}

static SDL_bool	ed_is_flat_selected(const t_map *map, const t_poly *poly)
{
	t_line	l1;
	t_line	l2;

	l1.p1 = (t_dot){poly->dots[0].x, poly->dots[0].y};
	l1.p2 = (t_dot){poly->dots[2].x, poly->dots[2].y};
	l2.p1 = (t_dot){poly->dots[1].x, poly->dots[1].y};
	l2.p2 = (t_dot){poly->dots[3].x, poly->dots[3].y};
	return (SDL_IntersectRectAndLine((SDL_Rect*)&map->editor.select_rect,
			&l1.p1.x, &l1.p1.y, &l1.p2.x, &l1.p2.y)
			||
			SDL_IntersectRectAndLine((SDL_Rect*)&map->editor.select_rect,
			&l2.p1.x, &l2.p1.y, &l2.p2.x, &l2.p2.y));
}

t_poly			*ed_get_selected_poly(t_map *map)
{
	t_poly	*poly;

	poly = NULL;
	if (map)
		poly = map->polys;
	while (poly)
	{
		if (ed_is_poly_printable(map, poly))
		{
			if ((ed_is_wall(poly) && ed_is_wall_selected(map, poly))
				||
				(ed_is_flat(poly) && ed_is_flat_selected(map, poly)))
				return (poly);
		}
		poly = poly->next;
	}
	return (NULL);
}
