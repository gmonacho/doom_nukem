#include "doom_nukem.h"

int				raycasting(t_win *win, t_player *player)
{
	t_calculs	calculs;

	calculs.raycast = 1;
	calculs.dangle = player->fov / win->w;
	calculs.dangle_up = player->fov_up / win->h;
	calculs.alpha = player->dir - player->fov / 2;
	if (calculs.alpha < 0)
		calculs.alpha += _2_PI;
	calculs.column = -1;
	// printf("Angle player : %f pi\n", player->dir / M_PI);
	while (++(calculs.column) < win->w)
	{
		// printf("---------------------\n");
		calculs.ray.angle = calculs.alpha;
		set_ray_equation(win, player, &(calculs.ray), player->pos);
		// printf("Player : %f\t%f\n", player->pos.x, player->pos.y);
		// printf("Source : %f\t%f\n\n", player->pos.y, calculs.ray.a * player->pos.x + calculs.ray.b);
		
		launch_ray_2d(win, player, &calculs);
		////////////// launch_ray_3d(win, player, &calculs);
		calculs.alpha += calculs.dangle;
		if (calculs.alpha > _2_PI)
			calculs.alpha -= _2_PI;
		// calculs.alpha += calculs.dangle;	//Deux lignes
		// (calculs.column)++;				//pour opti x2
	}
	//printf("Pos : %f\t%f\n", player->pos.x, player->pos.y);
	return (0);
}