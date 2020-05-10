/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 12:23:08 by widrye            #+#    #+#             */
/*   Updated: 2020/05/10 12:51:06 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "ui_error.h"

int			fill_poly_object_norm(char *tmp, t_poly *poly_object)
{
	if (!(poly_object->texture = IMG_Load(tmp)))
	{
		ui_ret_error("fill_poly_object", SDL_GetError(), 0);
		return (-1);
	}
	if (!(poly_object->texture = SDL_ConvertSurfaceFormat(
		poly_object->texture, SDL_PIXELFORMAT_ARGB8888, 0)))
	{
		ui_ret_error("fill_poly_object", SDL_GetError(), 0);
		return (-1);
	}
	return (0);
}

char		**lst_to_tab(t_list *lst, int height)
{
	int		i;
	char	**tab;
	t_list	*l;

	i = 0;
	l = lst;
	if (!(tab = (char **)malloc(sizeof(char *) * (height + 1))))
		return (NULL);
	tab[height] = NULL;
	while (l && i < height)
	{
		if (!(tab[i] = ft_strdup(l->content)))
			return (NULL);
		i++;
		l = l->next;
	}
	ft_free_list(lst);
	return (tab);
}

char		**ft_fill_map(int fd)
{
	char	*line;
	t_list	*lst;
	int		i;
	int		ret;

	i = 0;
	lst = NULL;
	while ((ret = get_next_line(fd, &line)) != 0)
	{
		if (ret == -1 || (lst = ft_lst_pb(&lst, line)) == NULL)
		{
			ft_free_list(lst);
			return (NULL);
		}
		i++;
	}
	return (lst_to_tab(lst, i));
}

char		**fillntesttab(int fd)
{
	char		**tab;
	int			i;
	static char	tmp[13];

	tmp[12] = '\0';
	i = 0;
	if ((read(fd, tmp, 12)) > 0)
	{
		if (!(ft_strcmp(tmp, "#GAMEREADY#\n")))
			create_tmp_bin_files(fd);
		else if (ft_strcmp(tmp, "\n###########") != 0)
		{
			ft_putendl("error map syntax");
			return (NULL);
		}
	}
	tab = NULL;
	tab = ft_fill_map(fd);
	if (ft_parse_error(tab) == (-1))
		return (NULL);
	return (tab);
}

int			fill_poly(t_poly *poly, t_map *map)
{
	if (fill_poly_mob(poly, map->mob) == -1 ||
	fill_poly_object(poly, map->objects) == -1)
		return (-1);
	ft_putendl("Fin parsing\n");
	return (0);
}
