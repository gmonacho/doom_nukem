#include "doom_nukem.h"

void				set_cartesienne(t_cartesienne *ray, t_fdot_3d origin, double alpha, double alpha_up)
{
	ray->ox = origin.x;
	ray->oy = origin.y;
	ray->oz = origin.z;
	ray->vx = cos(alpha_up) * cos(alpha);
	ray->vy = cos(alpha_up) * sin(alpha);
	ray->vz = sin(alpha_up);
}

void				set_cartesienne_static(t_cartesienne *ray, t_fdot_3d origin, double alpha, double alpha_up)
{
	ray->ox = origin.x;
	ray->oy = origin.y;
	ray->oz = origin.z;
	ray->vx = cos(alpha_up) * cos(alpha);
	ray->vy = cos(alpha_up) * sin(alpha);
	ray->vz = sin(alpha_up);
}






static int			create_ray(t_player *player, t_cartesienne **ray, t_fdot angle, t_dot coord)
{
	if (!(*ray = (t_cartesienne *)malloc(sizeof(t_cartesienne))))
		return (1);
	(*ray)->x = coord.x;
	(*ray)->y = coord.y;
	(*ray)->next = NULL;
	set_cartesienne_static(*ray, 	player->pos_up,	player->dir + angle.x,\
													player->dir_up + angle.y);
	// printf("Ray %f %f %f\t|\tAngle %f %f\n", (*ray)->vx, (*ray)->vy, (*ray)->vz, angle.x / M_PI, angle.y / M_PI);
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










void				set_origin_rays(t_cartesienne *rays, t_fdot_3d origin)
{
	rays->ox = origin.x;
	rays->oy = origin.y;
	rays->oz = origin.z;
}

void				rotate(t_cartesienne *line, t_matrice matrice)
{
	double			save_vx;
	double			save_vy;

	// printf("%f %f %f\n", line->vx, line->vy, line->vz);
	save_vx = line->vx * matrice._00 + line->vy * matrice._10 + line->vz * matrice._20;
	save_vy = line->vx * matrice._01 + line->vy * matrice._11 + line->vz * matrice._21;
	line->vz = line->vx * matrice._02 + line->vy * matrice._12 + line->vz * matrice._22;
	line->vx = save_vx;
	line->vy = save_vy;
	// printf("%f %f %f\n\n", line->vx, line->vy, line->vz);
}

void				rotate_all(t_cartesienne *line, t_matrice matrice)
{
	while (line)
	{
		rotate(line, matrice);
		line = line->next;
	}
}
