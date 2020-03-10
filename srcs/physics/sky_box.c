/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sky_box.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widrye <widrye@student.le-101.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 17:12:01 by agiordan          #+#    #+#             */
/*   Updated: 2020/03/10 10:17:50 by widrye           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static int		sphere_mapping(t_sphere *sphere, t_fdot_3d collision) //modifié pour que l'horion soit comme sur l'image de depart		#widrye
{
	t_fdot_3d	normal_dot;
	float		u;
	float		v;
	float		tmp;

	normal_dot = normalize(fdot_3d_sub(collision, sphere->pos));
	tmp = acos(-scalar_product(sphere->i, normal_dot));	// j'ai remplacé sphere->j par sphere->j		#widrye
	v = tmp / M_PI;
	tmp = scalar_product(normal_dot, sphere->j) / sin(tmp); // j'ai remplacé sphere->i par sphere->i		#widrye
	if (tmp < -1.0f)
		tmp = -1.0f;
	tmp = ((float)acos(tmp)) / _2_PI;
	u = (scalar_product(vectoriel_product(sphere->j, sphere->i),\
						normal_dot) > 0) ? tmp : 1 - tmp;
	if (is_null(u - 1, 0.000005))
		u = 0;
	if (is_null(v - 1, 0.000005))
		v = 0;
	if (u < 0 || u > 1 || v < 0 || v > 1)
		exit(0);
	return (((int *)sphere->texture->pixels)[(int)(v * sphere->texture->h) *\
						sphere->texture->w + (int)(u * sphere->texture->w)]);
}

int				sky_box(t_sphere *sky_box, t_cartesienne *ray)
{
	float		t1;
	float		t2;

	if (!resolve_polynome((t_fdot_3d){ray->vx * ray->vx +\
									ray->vy * ray->vy +\
									ray->vz * ray->vz,\
								-2 * (sky_box->pos.x * ray->vx +\
									sky_box->pos.y * ray->vy +\
									sky_box->pos.z * ray->vz),\
								sky_box->pos.x * sky_box->pos.x +\
									sky_box->pos.y * sky_box->pos.y +\
									sky_box->pos.z * sky_box->pos.z -\
									sky_box->radius * sky_box->radius},\
						&t1, &t2))
		return (0xFF505050);
	if (t1 < 0)
		t1 = t2;
	return (sphere_mapping(sky_box, (t_fdot_3d){ray->vx * t1,\
												ray->vy * t1,\
												ray->vz * t1}));
}
