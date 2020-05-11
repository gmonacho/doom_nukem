/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotations_x.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiordan <agiordan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 17:24:12 by agiordan          #+#    #+#             */
/*   Updated: 2020/05/11 02:08:52 by agiordan         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void				init_matrix_rx(t_player *player)
{
	player->rx.index_00 = 1;
	player->rx.index_01 = 0;
	player->rx.index_02 = 0;
	player->rx.index_10 = 0;
	player->rx.index_11 = cos(player->ddir);
	player->rx.index_12 = -sin(player->ddir);
	player->rx.index_20 = 0;
	player->rx.index_21 = sin(player->ddir);
	player->rx.index_22 = cos(player->ddir);
}

void				init_matrix_rx_inv(t_player *player)
{
	player->rx_inv.index_00 = 1;
	player->rx_inv.index_01 = 0;
	player->rx_inv.index_02 = 0;
	player->rx_inv.index_10 = 0;
	player->rx_inv.index_11 = cos(-player->ddir);
	player->rx_inv.index_12 = -sin(-player->ddir);
	player->rx_inv.index_20 = 0;
	player->rx_inv.index_21 = sin(-player->ddir);
	player->rx_inv.index_22 = cos(-player->ddir);
}

t_matrix			create_rx_matrix(float angle)
{
	return ((t_matrix){1, 0, 0,\
						0, cos(-angle), -sin(-angle),\
						0, sin(-angle), cos(-angle)});
}
