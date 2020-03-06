/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sky_box.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiordan <agiordan@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 17:12:01 by agiordan          #+#    #+#             */
/*   Updated: 2020/03/06 18:57:06 by agiordan         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void			fill_sky_box(t_map *map, char **tab, int i)
{
	char		*tmp;

	map->sky_box.pos = (t_fdot_3d){0, 0, 0};
	while ((ft_strchr(tab[i], '}') == NULL))
	{
		if (ft_strstr(tab[i], "texture = "))
		{
			tmp = ft_strjoin("textures/", ft_strrchr(tab[i], '=') + 2);
			if (!(map->sky_box.texture = IMG_Load(tmp)))
			{
				exit(0);
				return ;
			}
			map->sky_box.texture =\
							SDL_ConvertSurfaceFormat(map->sky_box.texture,\
													SDL_PIXELFORMAT_ARGB8888, 0);
			ft_strdel(&tmp);
		}
		else if (ft_strstr(tab[i], "radius = "))
			map->sky_box.radius = ft_atoi(ft_strrchr(tab[i], '=') + 2);
		i++;
	}
}

static int		sphere_mapping(t_sphere *sphere, t_fdot_3d collision)
{
	t_fdot_3d	normal;
	float		u;
	float		v;

	normal = normalize(fdot_3d_sub(sphere->pos, collision));
	u = 0.5 + atan2(normal.z, normal.x) / _2_PI;
	v = 0.5 - asin(normal.y) / M_PI;
	return (((int *)sphere->texture->pixels)[(int)((v * sphere->texture->h) *\
							sphere->texture->w + (u * sphere->texture->w))]);
}

int				sky_box(t_sphere *sky_box, t_cartesienne *ray)
{
	float		t1;
	float		t2;

	resolve_polynome((t_fdot_3d){ray->vx * ray->vx +\
									ray->vy * ray->vy +\
									ray->vz * ray->vz,\
								-2 * (sky_box->pos.x * ray->vx +\
									sky_box->pos.y * ray->vy +\
									sky_box->pos.z * ray->vz),\
								sky_box->pos.x * sky_box->pos.x +\
									sky_box->pos.y * sky_box->pos.y +\
									sky_box->pos.z * sky_box->pos.z -\
									sky_box->radius * sky_box->radius},\
						&t1, &t2);
	if (t1 < 0)
		t1 = t2;
	return (sphere_mapping(sky_box, (t_fdot_3d){ray->vx * t1,\
												ray->vy * t1,\
												ray->vz * t1}));
}
