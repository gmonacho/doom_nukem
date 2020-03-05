/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions_sphere.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiordan <agiordan@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 17:10:22 by agiordan          #+#    #+#             */
/*   Updated: 2020/03/05 18:15:10 by agiordan         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

/*
**	Code segment : 2 bits par segment 0bxxyy 0 <= xx <= 3
*/

int				is_in_segment(t_fdot_3d is, t_fdot_3d d1, t_fdot_3d d2)
{
	if (!is_null(d2.x - d1.x, 0.0005))
	{
		if (!(is.x > d1.x || is.x > d2.x) ||\
			!(is.x < d1.x || is.x < d2.x))
			return (0);
		return (1);
	}
	else if (!is_null(d2.y - d1.y, 0.0005))
	{
		if (!(is.y > d1.y || is.y > d2.y) ||\
			!(is.y < d1.y || is.y < d2.y))
			return (0);
		return (1);
	}
	else
	{
		if (!(is.z > d1.z || is.z > d2.z) ||\
			!(is.z < d1.z || is.z < d2.z))
			return (0);
		return (1);
	}
}

int				collision_dots(t_fdot_3d dots[N_DOTS_POLY], float ray)
{
	if (mag(dots[0]) <= ray ||\
		mag(dots[1]) <= ray ||\
		mag(dots[2]) <= ray ||\
		mag(dots[3]) <= ray)
		return (1);
	return (0);
}

int				collision_segment(t_fdot_3d dots[4], float width_2)
{
	t_fdot_3d	proj_ortho;

	if (is_in_segment(proj_ortho_origin_line(dots[0], dots[1], &proj_ortho),\
						dots[0], dots[1]) &&\
		mag(proj_ortho) <= width_2)
		return (0b0001);
	if (is_in_segment(proj_ortho_origin_line(dots[1], dots[2], &proj_ortho),\
						dots[1], dots[2]) &&\
		mag(proj_ortho) <= width_2)
		return (0b0110);
	if (is_in_segment(proj_ortho_origin_line(dots[2], dots[3], &proj_ortho),\
						dots[2], dots[3]) &&\
		mag(proj_ortho) <= width_2)
		return (0b1011);
	if (is_in_segment(proj_ortho_origin_line(dots[3], dots[0], &proj_ortho),\
						dots[3], dots[0]) &&\
		mag(proj_ortho) <= width_2)
		return (0b1100);
	return (0);
}

int				collision_poly(t_map *map, t_player *player, t_poly *poly)
{
	t_fdot_3d	proj_ortho;

	proj_ortho_plan((t_fdot_3d){0, 0, 0}, poly->equation_rotz_only,\
					&proj_ortho);
	if (mag(proj_ortho) > player->width_2)
		return (0);
	if (is_in_poly_rotz_only(poly, proj_ortho))
		return (1);
	if ((poly->segment_code = collision_segment(poly->dots_rotz_only,\
												player->width_2)) ||\
		collision_dots(poly->dots_rotz_only, map->player.width_2))
		return (1);
	return (0);
}

t_poly			*collisions_sphere(t_map *map, t_player *player,\
									t_poly *poly, int ban_interest)
{
	translate_all_rotz_only(map, map->polys,\
							(t_fdot_3d){0, 0, map->player._4_height_10});
	while (poly)
	{
		if (((poly->object && poly->object->collide) ||\
			(!poly->object && poly->collide) ||\
			(poly->object && poly->object->type == DOOR && poly->collide)) &&\
			(!ban_interest || !poly->is_slide_ban) &&\
			collision_poly(map, player, poly))
		{
			translate_all_rotz_only(map, map->polys,\
								(t_fdot_3d){0, 0, -map->player._4_height_10});
			return (poly);
		}
		poly = poly->next;
	}
	translate_all_rotz_only(map, map->polys,\
								(t_fdot_3d){0, 0, -map->player._4_height_10});
	return (NULL);
}
