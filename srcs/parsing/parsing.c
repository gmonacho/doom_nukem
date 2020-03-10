/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aducimet <aducimet@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:04:39 by aducimet          #+#    #+#             */
/*   Updated: 2020/03/10 18:04:41 by aducimet         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "ui_error.h"

int		find_texture(char *tab, t_poly *poly)
{
	char *tmp;
	char *name;

	name = NULL;
	tmp = NULL;
	name = ft_strdup(ft_strrchr(tab, '=') + 2);
	poly->texture_name = name;
	tmp = ft_strdup("textures/");
	tmp = ft_strjoin(tmp, name);
	ft_strdel(&name);
	if (!(poly->texture = IMG_Load(tmp)))
	{
		ft_strdel(&name);
		return (-1);
	}
	poly->texture = SDL_ConvertSurfaceFormat(poly->texture,
					SDL_PIXELFORMAT_ARGB8888, 0);
	free(tmp);
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
				return (-1);
		if (ft_strstr(tab[i], "light = "))
			(*poly)->light_coef = ft_atoi(ft_strrchr(tab[i], '=') + 1) / 100.0;
	}
	return (0);
}

int		fill_poly_mob(t_poly *poly, t_mob *mob)
{
	char *tmp;

	tmp = NULL;
	while (poly && poly->next)
		poly = poly->next;
	while (mob)
	{
		tmp = ft_strdup("textures/");
		poly->next = mob->poly;
		poly->light_coef = mob->light_coef;
		poly = poly->next;
		tmp = ft_strjoin(tmp, mob->texture);
		if (!(poly->texture = IMG_Load(tmp)))
		{
			ft_strdel(&tmp);
			return (-1);
		}
		poly->texture = SDL_ConvertSurfaceFormat(poly->texture,
						SDL_PIXELFORMAT_ARGB8888, 0);
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
	if ((tab = fillntesttab(fd)) == NULL)
		return (NULL);
	win->texhud = define_texhud(win);
	while (tab[++i])
		ft_data_storing2(&poly, map, tab, i);
	ft_2dstrdel(&tab);
	if (fill_poly(poly, map) == -1)
		return (NULL);
	return (poly);
}
