/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aducimet <aducimet@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:05:10 by aducimet          #+#    #+#             */
/*   Updated: 2020/03/10 18:05:13 by aducimet         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int			ft_data_storing2(t_poly **poly, t_map *map, char **tab, int i)
{
	if (ft_strstr(tab[i], "Polygon"))
		ft_fill_data(tab, poly, i);
	else if (ft_strstr(tab[i], "Object"))
	{
		if (object_data(tab, &(map->objects), i))
			return (-1);
	}
	else if (ft_strstr(tab[i], "Player"))
		player_data(tab, &(map->player), i);
	if (ft_strstr(tab[i], "Sphere"))
		new_sphere(&(map->sky_box), tab, i);
	else if (ft_strstr(tab[i], "Mob"))
	{
		if (fill_mob_data(&(map->mob), tab, i) == -1)
			return (-1);
	}
	return (0);
}

void		clear_leaks_obj(t_map *map)
{
	t_object	*object_tmp_next;

	if (map->objects)
	{
		while (map->objects)
		{
			object_tmp_next = map->objects->next;
			free(map->objects);
			map->objects = object_tmp_next;
		}
		map->objects = NULL;
	}
}

void		clear_leaks(t_map *map)
{
	t_poly		*poly_tmp_next;
	t_mob		*mob_tmp_next;

	if (map->polys)
	{
		while (map->polys)
		{
			poly_tmp_next = map->polys->next;
			free(map->polys);
			map->polys = poly_tmp_next;
		}
		map->polys = NULL;
	}
	if (map->mob)
	{
		while (map->mob)
		{
			mob_tmp_next = map->mob->next;
			free(map->mob);
			map->mob = mob_tmp_next;
		}
		map->mob = NULL;
	}
}
