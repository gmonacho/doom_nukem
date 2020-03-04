#include "doom_nukem.h"


static int		do_segs_intersect(t_line l1, t_line l2)
{
	float		s;
	float		t;
	t_fdot		d1;
	t_fdot		d2;
	float		divdis;

	d1 = (t_fdot){l1.p2.x - l1.p1.x, l1.p2.y - l1.p1.y};
	d2 = (t_fdot){l2.p2.x - l2.p1.x, l2.p2.y - l2.p1.y};
	if (!(divdis = -d2.x * d1.y + d1.x * d2.y))
		divdis += 0.0001; // evite de diviser par 0
	s = (-d1.y * (l1.p1.x - l2.p1.x) + d1.x * (l1.p1.y - l2.p1.y)) / divdis;
	t = (d2.x * (l1.p1.y - l2.p1.y) - d2.y * (l1.p1.x - l2.p1.x)) / divdis;
	if (s > 0 && s < 1 && t > 0 && t < 1)
	// && (l1.p1.x + (t * d1.x) != l2.p1.x
	// && l1.p1.y + (t * d1.y) != l2.p1.y))
		return (1);
	return (0);
}

static t_rect	ed_get_selected_rect(const t_rect *rect)
{
	t_rect	selected_rect;

	selected_rect = (t_rect){0, 0, 0, 0};
	if (rect)
	{
		selected_rect = *rect;
		if (selected_rect.w < 0)
		{
			selected_rect.w *= -1;
			selected_rect.x -= selected_rect.w;
		}
		if (selected_rect.h < 0)
		{
			selected_rect.h *= -1;
			selected_rect.y -= selected_rect.h;
		}
	}
	return (selected_rect);
}

int			rect_intersect_poly(t_map *map, t_poly poly)
{
	t_line	l1;
	t_line	l2;
	t_rect	rect;
	t_line	rect_diag;

	rect = ed_get_selected_rect(&map->editor.select_rect);
	l1.p1 = (t_dot){poly.dots[0].x, poly.dots[0].y}; 
	l1.p2 = (t_dot){poly.dots[2].x, poly.dots[2].y};
	l2.p1 = (t_dot){poly.dots[1].x, poly.dots[1].y};
	l2.p2 = (t_dot){poly.dots[3].x, poly.dots[3].y};
	rect_diag.p1 = (t_dot){rect.x, rect.y};
	rect_diag.p2 = (t_dot){rect.x + rect.w, rect.y + rect.h};
	if (do_segs_intersect(rect_diag, l1) || do_segs_intersect(rect_diag, l2))
		return (1);
	rect_diag.p1 = (t_dot){rect.x + rect.w, rect.y};
	rect_diag.p2 = (t_dot){rect.x, rect.y + rect.h};
	if (do_segs_intersect(rect_diag, l1) || do_segs_intersect(rect_diag, l2))
		return (1);
	return (0);
}

static SDL_bool	ed_is_wall_selected(const t_map *map, const t_poly poly)
{

	t_line	l;
	t_line	rect_side[6];
	t_rect	rect;
	int i;

	l.p1 = (t_dot){poly.dots[0].x, poly.dots[0].y};
	l.p2 = (t_dot){poly.dots[1].x, poly.dots[1].y};
	rect = ed_get_selected_rect(&map->editor.select_rect);
	rect_side[0].p1 = (t_dot){rect.x, rect.y};
	rect_side[0].p2 = (t_dot){rect.x + rect.w, rect.y};
	rect_side[1].p1 = (t_dot){rect.x + rect.w, rect.y};
	rect_side[1].p2 = (t_dot){rect.x + rect.w, rect.y + rect.h};
	rect_side[2].p1 = (t_dot){rect.x + rect.w, rect.y + rect.h};
	rect_side[2].p2 = (t_dot){rect.x, rect.y + rect.h};
	rect_side[3].p1 = (t_dot){rect.x, rect.y + rect.h};
	rect_side[3].p2 = (t_dot){rect.x, rect.y};
	rect_side[4].p1 = (t_dot){rect.x, rect.y + rect.h};
	rect_side[4].p2 = (t_dot){rect.x + rect.w, rect.y};
	rect_side[5].p1 = (t_dot){rect.x, rect.y};
	rect_side[5].p2 = (t_dot){rect.x + rect.w, rect.y + rect.h};
	i = -1;
	while (++i < 6)
		if (do_segs_intersect(rect_side[i], l))
			return (1);
	return (0);
	// return (SDL_IntersectRectAndLine((SDL_Rect*)&rect,
	// 		&p1.x, &p1.y, &p2.x, &p2.y));
}

// static SDL_bool	ed_is_flat_selected(const t_map *map, const t_poly *poly)
// {
// 	t_line	l1;
// 	t_line	l2;
// 	t_rect	rect;

// 	l1.p1 = (t_dot){poly->dots[0].x, poly->dots[0].y};
// 	l1.p2 = (t_dot){poly->dots[2].x, poly->dots[2].y};
// 	l2.p1 = (t_dot){poly->dots[1].x, poly->dots[1].y};
// 	l2.p2 = (t_dot){poly->dots[3].x, poly->dots[3].y};
// 	rect = ed_get_selected_rect(&map->editor.select_rect);
// 	return (SDL_IntersectRectAndLine((SDL_Rect*)&rect,
// 			&l1.p1.x, &l1.p1.y, &l1.p2.x, &l1.p2.y)
// 			||
// 			SDL_IntersectRectAndLine((SDL_Rect*)&rect,
// 			&l2.p1.x, &l2.p1.y, &l2.p2.x, &l2.p2.y));
// }

// static SDL_bool	ed_is_inclined_selected(t_win *win, const t_map *map, const t_poly *poly)
// {
// 	t_line	l1;
// 	t_line	l2;
// 	t_rect	rect;
	

// 	rect = ed_get_selected_rect(&map->editor.select_rect);
// 	return (rect_intersect_poly(rect, ))
// 	return (SDL_IntersectRectAndLine((SDL_Rect*)&rect,
// 			&l1.p1.x, &l1.p1.y, &l1.p2.x, &l1.p2.y)
// 			||
// 			SDL_IntersectRectAndLine((SDL_Rect*)&rect,
// 			&l2.p1.x, &l2.p1.y, &l2.p2.x, &l2.p2.y));
// 	// rect.w = ft_abs(poly->dots[2].x - rect.x);
// 	// rect.h = ft_abs(poly->dots[2].y - rect.y);
// 	// point = ed_get_map_point(map, win->winui->mouse.pos);
// 	// return (SDL_PointInRect((SDL_Point*)&point,
// 	// 						(SDL_Rect*)&rect));
// }

t_poly			*ed_get_selected_poly(t_win *win, t_map *map)
{
	t_poly	*poly;

	(void)win;
	poly = NULL;
	if (map)
	{
		poly = map->polys;
		while (poly)
		{
			if (ed_is_poly_printable(map, poly))
				if (ed_is_wall(poly) && ed_is_wall_selected(map, *poly))
					return (poly);
			poly = poly->next;
		}
		poly = map->polys;
		while (poly)
		{
			if (ed_is_poly_printable(map, poly))
				if (ed_is_flat(poly) && rect_intersect_poly(map, *poly))
					return (poly);
			poly = poly->next;
		}
		poly = map->polys;
		while (poly)
		{
			if (ed_is_poly_printable(map, poly))
				if (ed_is_inclined(poly) && rect_intersect_poly(map, *poly))
					return (poly);
			poly = poly->next;
		}
	}
	return (NULL);
}
