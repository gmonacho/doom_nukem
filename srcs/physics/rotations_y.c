#include "doom_nukem.h"

/*
**	ATTENTION les sin doivent doivent etre inverse par rapport aux matrices normales
*/

void				init_matrice_ry(t_player *player)
{
	player->ry._00 = cos(player->ddir);
	player->ry._01 = 0;
	player->ry._02 = -sin(player->ddir);
	player->ry._10 = 0;
	player->ry._11 = 1;
	player->ry._12 = 0;
	player->ry._20 = sin(player->ddir);
	player->ry._21 = 0;
	player->ry._22 = cos(player->ddir);
}

void				init_matrice_ry_inv(t_player *player)
{
	player->ry_inv._00 = cos(-player->ddir);
	player->ry_inv._01 = 0;
	player->ry_inv._02 = -sin(-player->ddir);
	player->ry_inv._10 = 0;
	player->ry_inv._11 = 1;
	player->ry_inv._12 = 0;
	player->ry_inv._20 = sin(-player->ddir);
	player->ry_inv._21 = 0;
	player->ry_inv._22 = cos(-player->ddir);
}
