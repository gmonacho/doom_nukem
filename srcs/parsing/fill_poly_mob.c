/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_poly_mob.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 17:57:38 by gal               #+#    #+#             */
/*   Updated: 2020/05/19 18:20:43 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void		assign_variables(char **tmp, t_poly *poly, t_mob *mob)
{
	char	*tmp_free;

	*tmp = ft_strdup("textures/");
	poly->next = mob->poly;
	poly->light_coef = mob->light_coef;
	poly = poly->next;
	tmp_free = *tmp;
	*tmp = ft_strjoin(*tmp, mob->texture);
	ft_strdel(&tmp_free);
}

int			fill_poly_mob(t_map *map, t_poly *poly, t_mob *mob)
{
	char	*tmp;

	tmp = NULL;
	while (poly && poly->next)
		poly = poly->next;
	while (mob)
	{
		assign_variables(&tmp, poly, mob);
		if (!is_id_in_stock(map->texture_stock, tmp))
			add_stock_texture(&map->texture_stock, new_stock_texture(tmp));
		poly->texture = get_surface_from_stock(map->texture_stock, tmp);
		mob = mob->next;
		ft_strdel(&tmp);
	}
	return (0);
}
