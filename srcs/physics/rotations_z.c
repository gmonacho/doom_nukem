#include "doom_nukem.h"

/*
**	ATTENTION les sin doivent doivent etre inverse par rapport aux matrices normales
*/

void				init_matrice_rz(t_player *player)
{
	player->rz._00 = cos(player->ddir);
	player->rz._01 = sin(player->ddir);
	player->rz._02 = 0;
	player->rz._10 = -sin(player->ddir);
	player->rz._11 = cos(player->ddir);
	player->rz._12 = 0;
	player->rz._20 = 0;
	player->rz._21 = 0;
	player->rz._22 = 1;
}

void				init_matrice_rz_inv(t_player *player)
{
	player->rz_inv._00 = cos(-player->ddir);
	player->rz_inv._01 = sin(-player->ddir);
	player->rz_inv._02 = 0;
	player->rz_inv._10 = -sin(-player->ddir);
	player->rz_inv._11 = cos(-player->ddir);
	player->rz_inv._12 = 0;
	player->rz_inv._20 = 0;
	player->rz_inv._21 = 0;
	player->rz_inv._22 = 1;
}
