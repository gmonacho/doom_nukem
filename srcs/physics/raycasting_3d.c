#include "doom_nukem.h"

/*
**	Attention a la teleportation du ray ! GALEEEEEREE
*/

static int			intersection_plan_line(t_player *player, t_calculs *calculs, t_plan plan, t_cartesienne line, t_fdot *collision)
{
	// if (calculs->column > 990)
	// {
	// 	printf("Equation plan : %f\n", plan.d);
	// 	printf("Equation line : %f %f %f\t%f %f %f\n", line.ox, line.oy, line.oz, line.vx, line.vy, line.vz);
	// }
	if (line.vz > -0.00001 && line.vz < 0.00001)
		return (1);
	collision->x = (-line.vx * (plan.d + player->sector->floor_height + player->height)) / line.vz + player->pos.x;
	collision->y = (-line.vy * (plan.d + player->sector->floor_height + player->height)) / line.vz + player->pos.y;
	// collision->x = player->pos.x + (line.vx - plan.d - (player->sector->floor_height + player->height)) / line.vz;
	// collision->y = player->pos.y + (line.vy - plan.d - (player->sector->floor_height + player->height)) / line.vz;
	// if (calculs->column > 990)
	// 	printf("Collision = %f %f\n", collision->x, collision->y);
	calculs = NULL;
	return (0);
}

static t_fvector	get_vector_by_angle(t_calculs *calculs, double angle, int z_axis)
{
	// if (calculs->column > 990)
	// {
	// 	printf("2 : %f Negatif ?\n", angle / M_PI);
	// 	if (angle < M_PI_2 || angle > _3_PI_2)
	// 	{
	// 		printf("3 : %f Positif !\n", angle / M_PI);
	// 	}
	// }
	t_fvector tmp = (t_fvector){cos(angle), sin(angle)};
	// if (calculs->column > 990)
	// 	printf("tmp = %f %f\n", tmp.x, tmp.y);
	return (tmp);
	// return ((t_fvector){	(angle < M_PI_2				||	angle > _3_PI_2)			?	cos(angle) : -cos(angle),
	// 						((angle > M_PI && !z_axis)	||	(angle < M_PI && z_axis))	?	sin(angle) : -sin(angle)});
	calculs = NULL;
	z_axis = 0;
}

static void			set_cartesienne(t_calculs *calculs, t_cartesienne *ray, t_fdot_3d origin, double dir, double dir_up)
{
	t_fvector	vector;

	vector = get_vector_by_angle(calculs, dir, 0);
	ray->vx = vector.x;
	ray->vy = vector.y;
	vector = get_vector_by_angle(calculs, dir_up, 1);
	ray->vz = vector.y;

	ray->ox = origin.x;
	ray->oy = origin.y;
	ray->oz = origin.z;
	// printf("vz = %f\n", ray->vz);
	// if (calculs->column > 990)
	// 	printf("dir = %fpi\tdir_up = %fpi\tvx vy vz : %f %f %f\n", dir / M_PI, dir_up / M_PI, ray->vx, ray->vy, ray->vz);
}

static void			draw_point(t_win *win, t_calculs *calculs, t_player *player, t_fdot dot, int y)
{
	Uint32	pixel;
	int		x;
	int		y_texture;

	x =			modulo(dot.x, player->sector->ceil_texture->w);
	y_texture =	modulo(dot.y, player->sector->ceil_texture->h);
	if (calculs->column > 990)
	{
	// 	printf("Player x y : %f %f\n", player->pos.x, player->pos.y);
		printf("x w : %f %d\n", dot.x, player->sector->ceil_texture->w);
		printf("y h : %f %d\n", dot.y, player->sector->ceil_texture->h);
		printf("Pixel : %d %d\n\n", x, y_texture);
		printf("Put pixel screen : %d %d\n\n", calculs->column, y);
	}
	pixel = ((Uint32 *)player->sector->ceil_texture->pixels)[y_texture * player->sector->ceil_texture->w + x];
	SDL_SetRenderDrawColor(win->rend,	(pixel >> 16) & 0xFF,\
										(pixel >> 8) & 0xFF,\
										(pixel >> 0) & 0xFF,\
										(pixel >> 24) & 0xFF);
	// if (!(y % 50))
	// 	printf("Up wall : %d, %f\n", y, calculs->up_wall);
	SDL_RenderDrawPoint(win->rend, calculs->column, y);
	calculs = NULL;
}

void				launch_ray_3d(t_win *win, t_player *player, t_calculs *calculs)
{
	int				z;
	double			angle;
	t_cartesienne	ray;
	t_fdot			collision;

	angle = player->dir_up + player->fov_up / 2;
	if (angle > _2_PI)
		angle -= _2_PI;
	z = -1;
	calculs->up_wall = win->middle_print + calculs->up_wall;
	while (++z < calculs->up_wall)
	{
		// printf("Debut launch 3d : %fpi %fpi %fpi\t%fpi\n", player->dir_up / M_PI, player->fov_up / M_PI, calculs->dangle_up / M_PI, angle / M_PI);
		set_cartesienne(calculs, &ray,\
						(t_fdot_3d){player->pos.x, player->pos.y, player->sector->floor_height + player->height},\
						calculs->alpha, angle);
		
		if (intersection_plan_line(player, calculs, player->sector->ceil_equation, ray, &collision))
		{
			printf("Parallole !!! : %d %d\n", calculs->column, z);
			continue ;
		}
			// printf("Dehors map ! %f %f\n", collision.x, collision.y);
		if (collision.x < 0)
			collision.x *= -1;
		if (collision.y < 0)
			collision.y *= -1;
		draw_point(win, calculs, player, collision, z);
		angle -= calculs->dangle_up;
		if (angle < 0)
			angle += _2_PI;
	}
	win = NULL;
}