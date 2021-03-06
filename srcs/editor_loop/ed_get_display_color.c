/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ed_get_display_color.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 14:21:09 by gal               #+#    #+#             */
/*   Updated: 2020/05/19 08:56:55 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

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
		if (inter == 0)
			return ((SDL_Color){0, 0, 255, 255});
		pourc = z * 100 / inter;
		if (pourc >= 50)
			return ((SDL_Color){255, 255 - (pourc - 50) * 255 / 50, 0, 255});
		else if (pourc == 0)
			return ((SDL_Color){0, 0, 255, 255});
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
		if (inter == 0)
			return ((SDL_Color){0, 0, 255, 255});
		pourc = z * 100 / inter;
		if (pourc >= 50)
			return ((SDL_Color){255, 255 - (pourc - 50) * 255 / 50, 0, 255});
		else if (pourc == 0)
			return ((SDL_Color){0, 0, 255, 255});
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
		if (inter == 0)
			return ((SDL_Color){0, 0, 255, 255});
		pourc = z * 100 / inter;
		if (pourc >= 50)
			return ((SDL_Color){255, 255 - (pourc - 50) * 255 / 50, 0, 255});
		else if (pourc == 0)
			return ((SDL_Color){0, 0, 255, 255});
		else if (pourc < 50)
		{
			return ((SDL_Color){255 - pourc * 255 / 50,
								255 - pourc * 255 / 50,
								255, 255});
		}
	}
	return ((SDL_Color){255, 255, 255, 255});
}

SDL_Color		ed_get_type_color(int type)
{
	if (type == HEAL)
		return ((SDL_Color){255, 50, 50, 255});
	else if (type == ARMOR)
		return ((SDL_Color){50, 50, 255, 255});
	else if (type == GRAVITY_INV)
		return ((SDL_Color){255, 150, 0, 255});
	else if (type == BULLET)
		return ((SDL_Color){255, 215, 0, 255});
	else if (type == BOX)
		return ((SDL_Color){100, 100, 100, 255});
	else if (type == DOOR)
		return ((SDL_Color){150, 120, 120, 255});
	else if (type == END)
		return ((SDL_Color){255, 100, 255, 255});
	else if (type == LIGHT)
		return ((SDL_Color){255, 255, 200, 255});
	return ((SDL_Color){255, 215, 0, 255});
}

SDL_Color		ed_get_obj_display_color(const t_map *map, t_object *obj)
{
	int			inter;
	int			pourc;
	int			z;

	if (map->editor.calc == ED_CALC_Z)
	{
		inter = ft_abs(map->editor.max_pos_z - map->editor.min_pos_z);
		z = ft_abs(obj->pos.z - map->editor.min_pos_z);
		if (inter == 0)
			return ((SDL_Color){0, 0, 255, 255});
		pourc = z * 100 / inter;
		if (pourc >= 50)
			return ((SDL_Color){255, 255 - (pourc - 50) * 255 / 50, 0, 255});
		else if (pourc == 0)
			return ((SDL_Color){0, 0, 255, 255});
		else if (pourc < 50)
		{
			return ((SDL_Color){255 - pourc * 255 / 50,
								255 - pourc * 255 / 50,
								255, 255});
		}
	}
	return (ed_get_type_color(obj->type));
}
