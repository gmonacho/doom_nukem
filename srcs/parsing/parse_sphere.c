/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiordan <agiordan@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 22:54:45 by agiordan          #+#    #+#             */
/*   Updated: 2020/03/07 21:21:10 by agiordan         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		fill_sphere_pos(t_sphere *sphere, char **tab, int i)
{
	if (ft_strstr(tab[i], "posx ="))
		sphere->pos.x = ft_atoi(ft_strrchr(tab[i], '=') + 1);
	else if (ft_strstr(tab[i], "posy ="))
		sphere->pos.y = ft_atoi(ft_strrchr(tab[i], '=') + 1);
	else if (ft_strstr(tab[i], "posz ="))
		sphere->pos.z = ft_atoi(ft_strrchr(tab[i], '=') + 1);
}

static void		fill_sphere(t_sphere *sphere, char **tab, int i)
{
	char		*tmp;

	while ((ft_strchr(tab[i], '}') == NULL))
	{
		fill_sphere_pos(sphere, tab, i);
		if (ft_strstr(tab[i], "texture = "))
		{
			tmp = ft_strjoin("textures/", ft_strrchr(tab[i], '=') + 2);
			if (!(sphere->texture = IMG_Load(tmp)))
			{
				printf("Texture sphere of the file failed\n");
				exit(0);
				return ;
			}
			sphere->texture = SDL_ConvertSurfaceFormat(\
							sphere->texture, SDL_PIXELFORMAT_ARGB8888, 0);
			ft_strdel(&tmp);
		}
		else if (ft_strstr(tab[i], "radius = "))
			sphere->radius = ft_atoi(ft_strrchr(tab[i], '=') + 2);
		i++;
	}
}

void			new_sphere(t_sphere *sphere, char **tab, int i)
{
	sphere->pos_rotz_only = (t_fdot_3d){0, 0, 0};
	sphere->pos = (t_fdot_3d){0, 0, 0};
	sphere->radius = 1000;
	if (!(sphere->texture = IMG_Load("textures/fractale1.jpg")))
	{
		printf("Default texture sphere failed\n");
		exit(0);
		return ;
	}
	sphere->i_rotz_only = (t_fdot_3d){1, 0, 0};
	sphere->i = (t_fdot_3d){1, 0, 0};
	sphere->j_rotz_only = (t_fdot_3d){0, 0, 1};
	sphere->j = (t_fdot_3d){0, 0, 1};
	fill_sphere(sphere, tab, i);
}

// static int		push_front_sphere(t_sphere **sphere)
// {
// 	t_sphere	*tmp;

// 	tmp = *sphere;
// 	if (!(*sphere = (t_sphere *)malloc(sizeof(t_sphere))))
// 		return (1);
// 	(*sphere)->next = tmp;
// 	return (0);
// }

// static void		fill_sphere_pos(t_sphere *sphere, char **tab, int i)
// {
// 	if (ft_strstr(tab[i], "posx ="))
// 		sphere->pos.x = ft_atoi(ft_strrchr(tab[i], '=') + 1);
// 	else if (ft_strstr(tab[i], "posy ="))
// 		sphere->pos.y = ft_atoi(ft_strrchr(tab[i], '=') + 1);
// 	else if (ft_strstr(tab[i], "posz ="))
// 		sphere->pos.z = ft_atoi(ft_strrchr(tab[i], '=') + 1);
// }

// static void		fill_sphere(t_sphere *sphere, char **tab, int i)
// {
// 	char		*tmp;

// 	while ((ft_strchr(tab[i], '}') == NULL))
// 	{
// 		fill_sphere_pos(sphere, tab, i);
// 		if (ft_strstr(tab[i], "texture = "))
// 		{
// 			tmp = ft_strjoin("textures/", ft_strrchr(tab[i], '=') + 2);
// 			if (!(sphere->texture = IMG_Load(tmp)))
// 			{
// 				exit(0);
// 				return ;
// 			}
// 			sphere->texture = SDL_ConvertSurfaceFormat(\
// 							sphere->texture, SDL_PIXELFORMAT_ARGB8888, 0);
// 			ft_strdel(&tmp);
// 		}
// 		else if (ft_strstr(tab[i], "radius = "))
// 			sphere->radius = ft_atoi(ft_strrchr(tab[i], '=') + 2);
// 		i++;
// 	}
// }

// int				new_sphere(t_sphere **sphere, char **tab, int i)
// {
// 	if (push_front_sphere(sphere))
// 		return (1);
// 	(*sphere)->pos_rotz_only = (t_fdot_3d){0, 0, 0};
// 	(*sphere)->pos = (t_fdot_3d){0, 0, 0};
// 	fill_sphere(*sphere, tab, i);
// 	return (0);
// }
