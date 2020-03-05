/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fts_poly.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiordan <agiordan@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 17:23:48 by agiordan          #+#    #+#             */
/*   Updated: 2020/03/05 17:23:51 by agiordan         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int			lstlen(t_poly *poly)
{
	int		count;

	count = 0;
	while (poly)
	{
		count++;
		poly = poly->next;
	}
	return (count);
}

void		copy_poly_lst(t_poly *dst, t_poly *src)
{
	t_poly	*tmp_next;
	int		tmp_is_slide_ban;

	while (src)
	{
		tmp_next = dst->next;
		tmp_is_slide_ban = dst->is_slide_ban;
		ft_memcpy(dst, src, sizeof(t_poly));
		dst->next = tmp_next;
		dst->is_slide_ban = tmp_is_slide_ban;
		dst = dst->next;
		src = src->next;
	}
}

int			create_poly_save(t_map *map)
{
	int		i;
	int		length;
	t_poly	*poly;

	if ((length = lstlen(map->polys)) &&\
		!(map->polys_save = (t_poly *)malloc(sizeof(t_poly))))
		return (1);
	poly = map->polys_save;
	i = 0;
	while (++i < length)
	{
		if (!(poly->next = (t_poly *)malloc(sizeof(t_poly))))
			return (1);
		poly = poly->next;
	}
	poly->next = NULL;
	return (0);
}
