#include "doom_nukem.h"

SDL_Color		ed_get_wall_display_color(const t_map *map, t_poly *poly)
{
	int			inter;
	int			pourc;
	int			z;

	if (map->editor.calc == ED_CALC_Z)
	{
		inter = ft_abs(map->editor.max_pos_z - map->editor.min_pos_z);
		z = ft_abs(poly->dots[1].z - map->editor.min_pos_z);
		pourc = z * 100 / inter;
		if (pourc >= 50)
			return ((SDL_Color){255, 255 - (pourc - 50) * 255 / 50, 0, 255});
		else if (pourc < 50)
		{
			return ((SDL_Color){255 - pourc * 255 / 50,
								255 - pourc * 255 / 50,
								255, 255});
		}
	}
	return ((SDL_Color){0, 0, 255, 255});
}


SDL_Color		ed_get_flat_display_color(const t_map *map, t_poly *poly)
{
	int			inter;
	int			pourc;
	int			z;

	if (map->editor.calc == ED_CALC_Z)
	{
		inter = ft_abs(map->editor.max_pos_z - map->editor.min_pos_z);
		z = ft_abs(poly->dots[0].z - map->editor.min_pos_z);
		printf("z = %d, inter = %d\n", z, inter);
		pourc = z * 100 / inter;
		if (pourc >= 50)
			return ((SDL_Color){255, 255 - (pourc - 50) * 255 / 50, 0, 255});
		else if (pourc < 50)
		{
			return ((SDL_Color){255 - pourc * 255 / 50,
								255 - pourc * 255 / 50,
								255, 255});
		}
	}
	return ((SDL_Color){0, 255, 0, 255});
}


SDL_Color		ed_get_mob_display_color(const t_map *map, t_mob *m)
{
	int			inter;
	int			pourc;
	int			z;
	if (map->editor.calc == ED_CALC_Z)
	{
		inter = ft_abs(map->editor.max_pos_z - map->editor.min_pos_z);
		z = ft_abs(m->pos.z - map->editor.min_pos_z);
		pourc = z * 100 / inter;
		if (pourc >= 50)
			return ((SDL_Color){255, 255 - (pourc - 50) * 255 / 50, 0, 255});
		else if (pourc < 50)
		{
			return ((SDL_Color){255 - pourc * 255 / 50,
								255 - pourc * 255 / 50,
								255, 255});
		}
	}
	return ((SDL_Color){255, 0, 255, 255});
}