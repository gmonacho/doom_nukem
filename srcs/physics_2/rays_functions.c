#include "doom_nukem.h"

// void				set_cartesienne(t_cartesienne *ray, double alpha, double alpha_up)
// {
// 	ray->vx = cos(alpha_up) * cos(alpha);
// 	ray->vy = cos(alpha_up) * sin(alpha);
// 	ray->vz = sin(alpha_up);
// }





static int			create_ray(t_player *player, t_cartesienne **ray, t_fdot angle, t_dot coord)
{
	double			alpha_up;
	double			alpha;

	alpha = 	player->dir + angle.x;
	alpha_up =	player->dir_up + angle.y;
	if (!(*ray = (t_cartesienne *)malloc(sizeof(t_cartesienne))))
		return (1);
	(*ray)->ox = 0;
	(*ray)->oy = 0;
	(*ray)->oz = 0;
	(*ray)->vx = cos(alpha_up) * cos(alpha);
	(*ray)->vy = cos(alpha_up) * sin(alpha);
	(*ray)->vz = sin(alpha_up);
	(*ray)->x = coord.x;
	(*ray)->y = coord.y;
	(*ray)->next = NULL;
	return (0);
}

int					init_rays(t_win *win, t_player *player)
{
	t_cartesienne	*ray;
	t_cartesienne	*ray_last;
	t_fdot			angle;
	t_fdot			dangle;
	t_dot			coord;

	init_matrice_rx(player);
	init_matrice_ry(player);
	init_matrice_rz(player);
	init_matrice_rx_inv(player);
	init_matrice_ry_inv(player);
	init_matrice_rz_inv(player);
	ray_last = NULL;
	ray = NULL;
	dangle = (t_fdot){player->fov / win->w, player->fov_up / win->h};
	angle.x = -player->fov / 2;
	coord.x = -1;
	while (++coord.x < win->w)
	{
		angle.y = player->fov_up / 2;
		coord.y = -1;
		while (++coord.y < win->h)
		{
			if (create_ray(player, &ray, angle, coord))
				return (1);
			// printf("%d %d|", coord.x, coord.y);
			if (ray_last)
				ray_last->next = ray;
			else
				player->rays = ray;
			ray_last = ray;
			angle.y -= dangle.y;
		}
		angle.x += dangle.x;
	}
	return (0);
}



void				rotate(t_fdot_3d *dot, t_matrice matrice)
{
	double			save_vx;
	double			save_vy;

	save_vx = dot->x * matrice._00 + dot->y * matrice._10 + dot->z * matrice._20;
	save_vy = dot->x * matrice._01 + dot->y * matrice._11 + dot->z * matrice._21;
	dot->z = dot->x * matrice._02 + dot->y * matrice._12 + dot->z * matrice._22;
	dot->x = save_vx;
	dot->y = save_vy;
}

void				rotate_all(t_sector *sector, t_matrice matrice)
{
	t_line			*line;

	while (sector)
	{
		line = sector->line;
		while (line)
		{
			rotate(&(line->equation.v), matrice);
			line = line->next;
		}
		sector = sector->next;
	}
}
