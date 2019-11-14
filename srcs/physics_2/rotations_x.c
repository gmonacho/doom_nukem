#include "doom_nukem.h"

/*
**	ATTENTION les sin doivent doivent etre inverse par rapport aux matrices normales
*/

void				init_matrice_rx(t_player *player)
{
	player->rx._00 = 1;
	player->rx._01 = 0;
	player->rx._02 = 0;
	player->rx._10 = 0;
	player->rx._11 = cos(player->ddir);
	player->rx._12 = -sin(player->ddir);
	player->rx._20 = 0;
	player->rx._21 = sin(player->ddir);
	player->rx._22 = cos(player->ddir);
}

void				init_matrice_rx_inv(t_player *player)
{
	player->rx_inv._00 = 1;
	player->rx_inv._01 = 0;
	player->rx_inv._02 = 0;
	player->rx_inv._10 = 0;
	player->rx_inv._11 = cos(-player->ddir);
	player->rx_inv._12 = -sin(-player->ddir);
	player->rx_inv._20 = 0;
	player->rx_inv._21 = sin(-player->ddir);
	player->rx_inv._22 = cos(-player->ddir);
}

t_matrice			create_matrice(double angle)
{
	return ((t_matrice){1, 0, 0, 0, cos(-angle), sin(-angle), 0, -sin(-angle), cos(-angle)});
}

