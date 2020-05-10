/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:04:39 by aducimet          #+#    #+#             */
/*   Updated: 2020/05/10 13:06:57 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "ui_error.h"

int		find_texture(char *tab, t_poly *poly)
{
	char	*tmp;
	char	*name;
	char	*tmp_free;

	name = NULL;
	tmp = NULL;
	name = ft_strdup(ft_strrchr(tab, '=') + 2);
	poly->texture_name = ft_strdup(name);
	tmp = ft_strdup("textures/");
	tmp_free = tmp;
	tmp = ft_strjoin(tmp, name);
	ft_strdel(&tmp_free);
	ft_strdel(&name);
	if (!(poly->texture = IMG_Load(tmp)))
	{
		ft_strdel(&name);
		return (ui_ret_error("find_texture", SDL_GetError(), -1));
	}
	if (!(poly->texture = SDL_ConvertSurfaceFormat(poly->texture,
					SDL_PIXELFORMAT_ARGB8888, 0)))
	{
		ft_strdel(&name);
		return (ui_ret_error("find_texture", SDL_GetError(), -1));
	}
	ft_strdel(&tmp);
	return (0);
}

int		ft_fill_data(char **tab, t_poly **poly, int i)
{
	int index;

	index = 0;
	add_poly(poly);
	(*poly)->object = NULL;
	(*poly)->mob = NULL;
	while ((ft_strchr(tab[i++], '}') == NULL))
	{
		if (ft_strstr(tab[i], "dot = "))
		{
			(*poly)->dots_rotz_only[index].x =
				ft_atoi(ft_strrchr(tab[i], 'x') + 2);
			(*poly)->dots_rotz_only[index].y =
				ft_atoi(ft_strrchr(tab[i], 'y') + 2);
			(*poly)->dots_rotz_only[index].z =
				ft_atoi(ft_strrchr(tab[i], 'z') + 2);
			index++;
		}
		if (ft_strstr(tab[i], "texture = "))
			if (find_texture(tab[i], *poly) == -1)
				return (ui_ret_error("ft_fill_data", "texture not found", -1));
		if (ft_strstr(tab[i], "light = "))
			(*poly)->light_coef = ft_atoi(ft_strrchr(tab[i], '=') + 1) / 100.0;
	}
	return (0);
}

int		fill_poly_mob(t_poly *poly, t_mob *mob)
{
	char	*tmp;
	char	*tmp_free;

	tmp = NULL;
	while (poly && poly->next)
		poly = poly->next;
	while (mob)
	{
		tmp = ft_strdup("textures/");
		poly->next = mob->poly;
		poly->light_coef = mob->light_coef;
		poly = poly->next;
		tmp_free = tmp;
		tmp = ft_strjoin(tmp, mob->texture);
		ft_strdel(&tmp_free);
		if (!(poly->texture = IMG_Load(tmp)))
		{
			ft_strdel(&tmp);
			return (-1);
		}
		if (!(poly->texture = SDL_ConvertSurfaceFormat(poly->texture,
						SDL_PIXELFORMAT_ARGB8888, 0)))
		{
			ft_strdel(&tmp);
			return (-1);
		}
		mob = mob->next;
		ft_strdel(&tmp);
	}
	return (0);
}

int		fill_poly_object(t_poly *poly, t_object *object)
{
	char	*tmp;
	t_poly	*poly_object;

	tmp = NULL;
	while (poly && poly->next)
		poly = poly->next;
	while (object)
	{
		tmp = ft_strjoin("textures/", object->texture);
		poly_object = object->poly;
		poly->light_coef = object->light_coef;
		while (poly_object)
		{
			if (fill_poly_object_norm(tmp, poly_object) == -1)
				return (-1);
			poly->next = poly_object;
			poly = poly->next;
			poly_object = poly_object->next;
		}
		ft_strdel(&tmp);
		object = object->next;
	}
	return (0);
}

t_poly	*ft_data_storing(int fd, t_map *map, t_win *win)
{
	char	**tab;
	int		i;
	t_poly	*poly;

	i = -1;
	poly = NULL;
	init_fpoly(&poly);
	map->mob = NULL;
	if (!(tab = fillntesttab(fd)))
		return (NULL);
	win->texhud = define_texhud(win);
	while (tab[++i])
	{
		if (!ft_data_storing2(&poly, map, tab, i) == -1)
			return (ui_ret_null_error("ft_data_storing", "2 failed", NULL));
	}
	ft_2dstrdel(&tab);
	if (fill_poly(poly, map) == -1)
		return (NULL);
	return (poly);
}
