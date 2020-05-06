/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ed_get_z_limits.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 14:21:18 by gal               #+#    #+#             */
/*   Updated: 2020/05/06 14:21:18 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		ed_get_z_min(const t_poly *polys)
{
	int				min;
	int				first;
	int				i;
	const t_poly	*p;

	p = polys;
	first = 1;
	while (p)
	{
		i = 0;
		while (i < 4)
		{
			if (first || p->dots[i].z < min)
			{
				min = p->dots[i].z;
				first = 0;
			}
			i++;
		}
		p = p->next;
	}
	return (min);
}

int		ed_get_z_max(const t_poly *polys)
{
	int				max;
	int				first;
	int				i;
	const t_poly	*p;

	p = polys;
	first = 1;
	while (p)
	{
		i = 0;
		while (i < 4)
		{
			if (first || p->dots[i].z > max)
			{
				max = p->dots[i].z;
				first = 0;
			}
			i++;
		}
		p = p->next;
	}
	return (max);
}
