/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 17:37:19 by gal               #+#    #+#             */
/*   Updated: 2020/05/19 09:23:30 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "ui_error.h"

void		free_find(char **s1, char **s2)
{
	if (s1)
		ft_strdel(s1);
	if (s2)
		ft_strdel(s2);
}

int			find_texture(char *tab, t_poly *poly)
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
	free_find(&tmp_free, &name);
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
