/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_poly_mob.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 17:57:38 by gal               #+#    #+#             */
/*   Updated: 2020/05/10 18:06:30 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

const void	assign_variables(char **tmp, t_poly *poly, t_mob *mob)
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

int			fill_poly_mob(t_poly *poly, t_mob *mob)
{
	char	*tmp;

	tmp = NULL;
	while (poly && poly->next)
		poly = poly->next;
	while (mob)
	{
		assign_variables(&tmp, poly, mob);
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
