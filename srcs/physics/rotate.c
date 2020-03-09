/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiordan <agiordan@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 18:06:50 by agiordan          #+#    #+#             */
/*   Updated: 2020/03/05 18:12:25 by agiordan         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void			rotate_box(t_player *player, t_object *object)
{
	t_poly		*poly;
	t_matrix	matrix;
	int			i;

	matrix = create_ry_matrix(-object->rot_y_save + player->rot_y);
	object->pos = rotate_dot(object->pos, matrix);
	poly = object->poly;
	i = -1;
	while (poly && ++i < 6)
	{
		poly->equation_rotz_only.v = rotate_dot(poly->equation_rotz_only.v,\
												matrix);
		poly->dots_rotz_only[0] = rotate_dot(poly->dots_rotz_only[0], matrix);
		poly->dots_rotz_only[1] = rotate_dot(poly->dots_rotz_only[1], matrix);
		poly->dots_rotz_only[2] = rotate_dot(poly->dots_rotz_only[2], matrix);
		poly->dots_rotz_only[3] = rotate_dot(poly->dots_rotz_only[3], matrix);
		poly->i_rotz_only = fdot_3d_sub(poly->dots_rotz_only[1],\
										poly->dots_rotz_only[0]);
		poly->j_rotz_only = fdot_3d_sub(poly->dots_rotz_only[N_DOTS_POLY - 1],\
										poly->dots_rotz_only[0]);
		poly = poly->next;
	}
}

t_fdot_3d		rotate_dot(t_fdot_3d dot, t_matrix matrix)
{
	return ((t_fdot_3d)\
			{dot.x * matrix._00 + dot.y * matrix._10 + dot.z * matrix._20,\
			dot.x * matrix._01 + dot.y * matrix._11 + dot.z * matrix._21,\
			dot.x * matrix._02 + dot.y * matrix._12 + dot.z * matrix._22});
}
