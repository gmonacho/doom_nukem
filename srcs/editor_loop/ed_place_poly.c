#include "doom_nukem.h"
#include "ui_error.h"

int		ed_place_wall(t_win *win, t_map *map)
{
	t_poly	*poly;
	t_dot	p;

	poly = NULL;
	p = ed_get_map_point(map, win->winui->mouse.pos);
	if (!map->editor.placing_poly)
	{
		if (!(poly = ft_memalloc(sizeof(t_poly))))
		{
			return (ui_ret_error("ed_place_wall",
			"placing_poly allocation failed", 0));
		}
		poly->dots[0] = (t_fdot_3d){p.x,
									p.y,
									map->editor.wall_max};
		poly->dots[1] = (t_fdot_3d){p.x,
									p.y,
									map->editor.wall_max};
		poly->dots[2] = (t_fdot_3d){p.x,
									p.y,
									map->editor.wall_min};
		poly->dots[3] = (t_fdot_3d){p.x,
									p.y,
									map->editor.wall_min};
		map->editor.placing_poly = poly;
	}
	else
	{
		map->editor.placing_poly->dots[1].x = p.x;
		map->editor.placing_poly->dots[1].y = p.y;
		map->editor.placing_poly->dots[2].x = p.x;
		map->editor.placing_poly->dots[2].y = p.y;
	}
	return (1);
}

// int		ed_place_flat(t_win *win, t_map *map)