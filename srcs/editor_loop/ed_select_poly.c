#include "doom_nukem.h"

static SDL_bool	ed_is_wall_selected(const t_map *map, const t_poly *poly)
{
	return (SDL_IntersectRectAndLine((SDL_Rect*)&map->editor.select_rect,
			(int*)&poly->dots[0].x, (int*)&poly->dots[0].y,
			(int*)&poly->dots[1].x, (int*)&poly->dots[1].y));
}

t_poly			*ed_get_selected_poly(t_map *map)
{
	t_poly	*poly;

	poly = NULL;
	if (map)
		poly = map->polys;
	while (poly)
	{
		if (ed_is_wall(poly) && ed_is_wall_selected(map, poly))
			return (poly);
		poly = poly->next;
	}
	return (NULL);
}
