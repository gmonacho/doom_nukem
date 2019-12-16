#include "doom_nukem.h"

/*
**	Mag ray = 1.000000
*/

static int			create_ray(t_cartesienne *ray, t_fdot angle, t_dot coord)
{
	ray->ox = 0;
	ray->oy = 0;
	ray->oz = 0;
	ray->vx = 1;
	ray->vy = angle.x;
	ray->vz = angle.y;
	// ray->vx = cos(angle.y) * cos(angle.x);
	// ray->vy = cos(angle.y) * sin(angle.x);
	// ray->vz = sin(angle.y);
	ray->dist = -1;
	ray->poly = NULL;
	ray->collision = (t_fdot_3d){};
	if ((coord.x == 0 && coord.y == 0) ||\
		(coord.x == 999 && coord.y == 0) ||\
		(coord.x == 0 && coord.y == 799) ||\
		(coord.x == 999 && coord.y == 799))
		printf("Ray (%f, %f, %f)\n", ray->vx, ray->vy, ray->vz);
	return (0);
}

int					init_rays(t_win *win, t_player *player)
{
	t_fdot			angle;
	t_fdot			dangle;
	t_dot			coord;

	init_matrix_rx(player);
	init_matrix_ry(player);
	init_matrix_rz(player);
	init_matrix_rx_inv(player);
	init_matrix_ry_inv(player);
	init_matrix_rz_inv(player);
	dangle = (t_fdot){player->fov / win->w, player->fov_up / win->h};
	if (!(player->rays = (t_cartesienne **)malloc(sizeof(t_cartesienne *) * (HEIGHT + 1))))
		return (1);
	player->rays[HEIGHT] = NULL;
	angle.y = player->fov_up / 2;
	coord.y = -1;
	while (++coord.y < win->h)
	{
		if (!(player->rays[coord.y] = (t_cartesienne *)malloc(sizeof(t_cartesienne) * (WIDTH))))
			return (1);
		angle.x = -player->fov / 2;
		coord.x = -1;
		while (++coord.x < win->w)
		{
			if (create_ray(&(player->rays[coord.y][coord.x]), angle, coord))
				return (1);
			angle.x += dangle.x;
		}
		angle.y -= dangle.y;
	}
	return (0);
}
