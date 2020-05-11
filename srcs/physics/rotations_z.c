/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotations_z.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiordan <agiordan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 17:24:16 by agiordan          #+#    #+#             */
/*   Updated: 2020/05/11 02:09:04 by agiordan         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void				init_matrix_rz(t_player *player)
{
	player->rz.index_00 = cos(player->ddir);
	player->rz.index_01 = -sin(player->ddir);
	player->rz.index_02 = 0;
	player->rz.index_10 = sin(player->ddir);
	player->rz.index_11 = cos(player->ddir);
	player->rz.index_12 = 0;
	player->rz.index_20 = 0;
	player->rz.index_21 = 0;
	player->rz.index_22 = 1;
}

void				init_matrix_rz_inv(t_player *player)
{
	player->rz_inv.index_00 = cos(-player->ddir);
	player->rz_inv.index_01 = -sin(-player->ddir);
	player->rz_inv.index_02 = 0;
	player->rz_inv.index_10 = sin(-player->ddir);
	player->rz_inv.index_11 = cos(-player->ddir);
	player->rz_inv.index_12 = 0;
	player->rz_inv.index_20 = 0;
	player->rz_inv.index_21 = 0;
	player->rz_inv.index_22 = 1;
}

t_matrix			create_rz_matrix(float angle)
{
	return ((t_matrix){cos(angle), -sin(angle), 0,\
						sin(angle), cos(angle), 0,\
						0, 0, 1});
}
