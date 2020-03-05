/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotations_z.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiordan <agiordan@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 17:24:16 by agiordan          #+#    #+#             */
/*   Updated: 2020/03/05 17:24:18 by agiordan         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void				init_matrix_rz(t_player *player)
{
	player->rz._00 = cos(player->ddir);
	player->rz._01 = -sin(player->ddir);
	player->rz._02 = 0;
	player->rz._10 = sin(player->ddir);
	player->rz._11 = cos(player->ddir);
	player->rz._12 = 0;
	player->rz._20 = 0;
	player->rz._21 = 0;
	player->rz._22 = 1;
}

void				init_matrix_rz_inv(t_player *player)
{
	player->rz_inv._00 = cos(-player->ddir);
	player->rz_inv._01 = -sin(-player->ddir);
	player->rz_inv._02 = 0;
	player->rz_inv._10 = sin(-player->ddir);
	player->rz_inv._11 = cos(-player->ddir);
	player->rz_inv._12 = 0;
	player->rz_inv._20 = 0;
	player->rz_inv._21 = 0;
	player->rz_inv._22 = 1;
}

t_matrix			create_rz_matrix(float angle)
{
	return ((t_matrix){cos(angle), -sin(angle), 0,\
						sin(angle), cos(angle), 0,\
						0, 0, 1});
}
