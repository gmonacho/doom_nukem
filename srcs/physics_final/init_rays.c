#include "doom_nukem.h"

static int			create_ray(t_cartesienne *ray, t_fdot angle, t_dot coord)
{
	ray->ox = 0;
	ray->oy = 0;
	ray->oz = 0;
	ray->vx = cos(angle.y) * cos(angle.x);
	ray->vy = cos(angle.y) * sin(angle.x);
	ray->vz = sin(angle.y);
	ray->dist = -1;
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

	init_matrice_rx(player);
	init_matrice_ry(player);
	init_matrice_rz(player);
	init_matrice_rx_inv(player);
	init_matrice_ry_inv(player);
	init_matrice_rz_inv(player);
	dangle = (t_fdot){player->fov / win->w, player->fov_up / win->h};
	angle.x = -player->fov / 2;
	coord.x = -1;
	while (++coord.x < win->w)
	{
		angle.y = player->fov_up / 2;
		coord.y = -1;
		while (++coord.y < win->h)
		{
			if (create_ray(&(player->rays[coord.y][coord.x]), angle, coord))
				return (1);
			angle.y -= dangle.y;
		}
		angle.x += dangle.x;
	}
	return (0);
}


t_poly				*polys_a_la_mano()		//En attendant le nouveau parsing
{
	t_poly		*save;
	t_poly		*poly;

	poly = malloc(sizeof(t_poly));
	save = poly;

	poly->d1 = (t_fdot_3d){100, 650, 200};
	poly->d2 = (t_fdot_3d){100, 150, 200};
	poly->d3 = (t_fdot_3d){100, 650, 50};
	poly->d4 = (t_fdot_3d){100, 150, 50};
	poly->next = malloc(sizeof(t_poly));
	poly = poly->next;

	poly->d1 = (t_fdot_3d){100, 150, 200};
	poly->d2 = (t_fdot_3d){600, 150, 200};
	poly->d3 = (t_fdot_3d){100, 150, 50};
	poly->d4 = (t_fdot_3d){600, 150, 50};
	poly->next = malloc(sizeof(t_poly));
	poly = poly->next;

	poly->d1 = (t_fdot_3d){600, 150, 200};
	poly->d2 = (t_fdot_3d){600, 650, 200};
	poly->d3 = (t_fdot_3d){600, 150, 50};
	poly->d4 = (t_fdot_3d){600, 650, 50};
	poly->next = malloc(sizeof(t_poly));
	poly = poly->next;

	poly->d1 = (t_fdot_3d){600, 650, 200};
	poly->d2 = (t_fdot_3d){100, 650, 200};
	poly->d3 = (t_fdot_3d){600, 650, 50};
	poly->d4 = (t_fdot_3d){100, 650, 50};
	poly->next = malloc(sizeof(t_poly));
	poly = poly->next;

	poly->d1 = (t_fdot_3d){100, 650, 50};
	poly->d2 = (t_fdot_3d){100, 150, 50};
	poly->d3 = (t_fdot_3d){600, 650, 50};
	poly->d4 = (t_fdot_3d){600, 150, 50};
	poly->next = malloc(sizeof(t_poly));
	poly = poly->next;

	poly->d1 = (t_fdot_3d){100, 650, 200};
	poly->d2 = (t_fdot_3d){100, 150, 200};
	poly->d3 = (t_fdot_3d){600, 650, 200};
	poly->d4 = (t_fdot_3d){600, 150, 200};
	poly->next = NULL;
	return (save);
}