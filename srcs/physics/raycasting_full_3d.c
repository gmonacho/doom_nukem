#include "doom_nukem.h"

/*
**	Attention a la teleportation du ray ! GALEEEEEREE
*/

static void			draw_point(t_win *win, t_calculs *calculs, t_player *player, int y)
{
	Uint32			pixel;
	int				x_texture;
	int				y_texture;

	if (calculs->collision_wall)
	{
		x_texture = modulo(	fdist(	(t_fdot){calculs->collision_wall->p1.x,\
											calculs->collision_wall->p1.y},
									(t_fdot){calculs->closest_2.x,\
											calculs->closest_2.y}),
							calculs->collision_wall->texture->w);
		y_texture = modulo(calculs->closest_2.z, calculs->collision_wall->sector->height -\
												calculs->collision_wall->texture->h);
	}
	else
	{
		x_texture =	modulo(calculs->closest_2.x, player->sector->ceil_texture->w);
		y_texture =	modulo(calculs->closest_2.y, player->sector->ceil_texture->h);
	}
	// if (calculs->column > 990)
	// {
	// 	printf("Player x y : %f %f\n", player->pos.x, player->pos.y);
		// printf("x w : %f %d\n", dot.x, player->sector->ceil_texture->w);
		// printf("y h : %f %d\n", dot.y, player->sector->ceil_texture->h);
		// printf("Pixel : %d %d\n\n", x, y_texture);
		// printf("Put pixel screen : %d %d\n\n", calculs->column, y);
	// }
	pixel = ((Uint32 *)player->sector->ceil_texture->pixels)[y_texture * player->sector->ceil_texture->w + x_texture];
	SDL_SetRenderDrawColor(win->rend,	(pixel >> 16) & 0xFF,\
										(pixel >> 8) & 0xFF,\
										(pixel >> 0) & 0xFF,\
										(pixel >> 24) & 0xFF);
	// if (!(y % 50))
	// 	printf("Up wall : %d, %f\n", y, calculs->up_wall);
	SDL_RenderDrawPoint(win->rend, calculs->column, y);
	calculs = NULL;
}


















static int			intersection_plan_line(t_fdot_3d source, t_calculs *calculs, t_plan plan, t_fdot_3d *collision)
{
	double			t;

	// if (calculs->column > 990)
	// {
	// 	printf("Equation plan : %f\n", plan.d);
	// 	printf("Equation line : %f %f %f\t%f %f %f\n", line.ox, line.oy, line.oz, line.vx, line.vy, line.vz);
	// }

	// if (line.vz > -0.00001 && line.vz < 0.00001)
	// 	return (1);
	
	t = -(plan.a * source.x + plan.b * source.y + plan.c * source.z + plan.d) /\
		(double)(plan.a * calculs->ray_2.vx + plan.b * calculs->ray_2.vy + plan.c * calculs->ray_2.vz);
	
	// t = (-source.x - source.y - source.z - plan.d) /\
	// 	(plan.a * calculs->ray_2.vx + plan.b * calculs->ray_2.vy + plan.c * calculs->ray_2.vz);
	
	// if (calculs->column >= 1000)
	// {
	// 	t = -(plan.a * source.x + plan.b * source.y + plan.c * source.z + plan.d) /\
	// 	(double)(plan.a * calculs->ray_2.vx + plan.b * calculs->ray_2.vy + plan.c * calculs->ray_2.vz);
	// 	printf("t = %f\n", t);
	// }

	collision->x = source.x + calculs->ray_2.vx * t;
	collision->y = source.y + calculs->ray_2.vy * t;
	collision->z = source.z + calculs->ray_2.vz * t;
	// collision->x = (-line.vx * (plan.d + player->sector->floor_height + player->height)) / line.vz + player->pos.x;
	// collision->y = (-line.vy * (plan.d + player->sector->floor_height + player->height)) / line.vz + player->pos.y;
	return (0);
}

// static t_fvector	get_vector_by_angle(t_calculs *calculs, double angle, int z_axis)
// {
// 	// if (calculs->column > 990)
// 	// {
// 	// 	printf("2 : %f Negatif ?\n", angle / M_PI);
// 	// 	if (angle < M_PI_2 || angle > _3_PI_2)
// 	// 	{
// 	// 		printf("3 : %f Positif !\n", angle / M_PI);
// 	// 	}
// 	// }
// 	// t_fvector tmp = (t_fvector){cos(angle), sin(angle)};
// 	// if (calculs->column > 990)
// 	// 	printf("tmp = %f %f\n", tmp.x, tmp.y);
// 	return ((t_fvector){cos(angle), sin(angle)});
// 	// return ((t_fvector){	(angle < M_PI_2				||	angle > _3_PI_2)			?	cos(angle) : -cos(angle),
// 	// 						((angle > M_PI && !z_axis)	||	(angle < M_PI && z_axis))	?	sin(angle) : -sin(angle)});
// 	calculs = NULL;
// 	z_axis = 0;
// }

static void			set_cartesienne(t_calculs *calculs, t_cartesienne *ray, t_fdot_3d origin)
{
	// t_fvector		vector;
	// t_fvector		vector1;
	// t_fvector		vector2;

	// vector1 = get_vector_by_angle(calculs, calculs->alpha_up, 1);
	// vector2 = get_vector_by_angle(calculs, calculs->alpha_tmp, 0);
	// ray->vz = vector1.y;
	// ray->vx = vector1.x * vector2.x;
	// ray->vy = vector1.x * vector2.y;

	ray->vx = cos(calculs->alpha_up) * cos(calculs->alpha_tmp);
	ray->vy = cos(calculs->alpha_up) * sin(calculs->alpha_tmp);
	ray->vz = sin(calculs->alpha_up);
	
	if (calculs->column >= 1000)
		printf("alpha up -> vz %f pi %f\n", calculs->alpha_up / M_PI, ray->vz);

	ray->ox = origin.x;
	ray->oy = origin.y;
	ray->oz = origin.z;
	// printf("vz = %f\n", ray->vz);
	// if (calculs->column > 990)
	// 	printf("dir = %fpi\tdir_up = %fpi\tvx vy vz : %f %f %f\n", dir / M_PI, dir_up / M_PI, ray->vx, ray->vy, ray->vz);
}











static t_linedef	*test_floor_ceil(t_calculs *calculs, t_fdot_3d source, t_sector *sector)
{
	t_fdot_3d		collision;
	double			tmpdist;

	if (intersection_plan_line(source, calculs, sector->floor_equation, &collision))
	{
		printf("Parallole !!! : %d\n", calculs->column);
		return (NULL);
	}
	if (((tmpdist = fdist_3d(source, collision)) < calculs->dist) &&\
		sence(calculs->ray_2, collision))
	{
		calculs->collision_wall = NULL;
		calculs->closest_2 = collision;
		calculs->dist = tmpdist;
	}
	if (intersection_plan_line(source, calculs, sector->ceil_equation, &collision))
	{
		printf("Parallole !!! : %d\n", calculs->column);
		return (NULL);
	}
	if ((((tmpdist = fdist_3d(source, collision)) < calculs->dist) &&\
		sence(calculs->ray_2, collision)))
	{
		calculs->collision_wall = NULL;
		calculs->closest_2 = collision;
		calculs->dist = tmpdist;
	}
	return (calculs->collision_wall);
}

static t_linedef	*launch_ray_3d(t_win *win, t_player *player, t_calculs *calculs, t_fdot_3d source, t_sector *sector)
{
	t_linedef		*line;
	t_fdot_3d		collision;
	double			tmpdist;

	calculs->dist = -1;
	calculs->collision_wall = NULL;
	line = sector->lines;
	// printf("closest begin %p\n", calculs->collision_wall);
	while (line)
	{
		if (intersection_plan_line(source, calculs, line->equation_2, &collision))
		{
			printf("Parallole !!! : %d\n", calculs->column);
			continue ;
		}
		if ((calculs->dist == -1 ||
			(tmpdist = fdist_3d(source, collision)) < calculs->dist) &&\
			sence(calculs->ray_2, collision) &&\
			((line->p1.x <= collision.x && collision.x <= line->p2.x) ||\
			(line->p2.x <= collision.x && collision.x <= line->p1.x)) &&\
			((int)collision.x != (int)source.x || (int)collision.y != (int)source.y))
		{
			calculs->collision_wall = line;
			calculs->closest_2 = collision;
			calculs->dist = tmpdist;
		}
		// printf("closest %p\n", calculs->collision_wall);
		// printf("Dehors map ! %f %f\n", collision.x, collision.y);
		// if (collision.x < 0)
		// 	collision.x *= -1;
		// if (collision.y < 0)
		// 	collision.y *= -1;
		line = line->next;
	}
	test_floor_ceil(calculs, source, sector);
	win = NULL;
	player = NULL;
	return (calculs->collision_wall);
}

static t_fdot_3d	begin_launch(t_win *win, t_player *player, t_calculs *calculs, int z)
{
	t_sector		*sector;
	t_fdot_3d		source;
	t_linedef		*wall;

	sector = player->sector;
	source = player->pos_up;
	set_cartesienne(calculs, &(calculs->ray_2), source);
	if (calculs->column == 0 && !z)
	{
		printf("Angles : %f %f\n", calculs->alpha_tmp / M_PI, calculs->alpha_up / M_PI);
		printf("Ray : %f %f %f\t%f %f %f\n", calculs->ray_2.ox, calculs->ray_2.oy, calculs->ray_2.oz, calculs->ray_2.vx, calculs->ray_2.vy, calculs->ray_2.vz);
	}
	wall = launch_ray_3d(win, player, calculs, source, sector);
	while (wall && wall->flags & PORTAL && calculs->dist < RENDER_DISTANCE)
	{
		printf("Wall collision : %p\tPortal ? %d\n", wall, wall->flags & PORTAL);
		source = (t_fdot_3d){calculs->closest_2.x, calculs->closest_2.y, calculs->closest_2.z};
		set_new_position_3d(&source, wall, wall->destline, &sector);
		set_ray_angle(&(calculs->alpha_tmp), wall, wall->destline);
		set_cartesienne(calculs, &(calculs->ray_2), source);
		wall = launch_ray_3d(win, player, calculs, source, sector);
	}
	return (calculs->closest_2);
}









static void			raycasting_vertical(t_win *win, t_player *player, t_calculs *calculs)
{
	int				z;
	// t_fdot_3d		collision;

	calculs->alpha_up = player->dir_up + player->fov_up / 2;
	if (calculs->alpha_up > _2_PI)
		calculs->alpha_up -= _2_PI;
	z = -1;
	while (++z < win->h)
	{
		// collision = begin_launch(win, player, calculs);
		begin_launch(win, player, calculs, z);
		draw_point(win, calculs, player, z);
		calculs->alpha_up -= calculs->dangle_up;
		if (calculs->alpha_up < 0)
			calculs->alpha_up += _2_PI;
	}
}







int				raycasting_3d(t_win *win, t_player *player)
{
	t_calculs	calculs;

	calculs.raycast = 1;
	calculs.alpha = player->dir - player->fov / 2;
	if (calculs.alpha < 0)
		calculs.alpha += _2_PI;
	calculs.dangle = player->fov / win->w;
	calculs.dangle_up = player->fov_up / win->h;

	calculs.column = -1;
	while (++(calculs.column) < win->w)
	{
		// printf("Dir a dirup aup %fpi %fpi\t%fpi %fpi\n", player->dir / M_PI, calculs.alpha / M_PI, player->dir_up / M_PI, calculs.alpha_up / M_PI);
		calculs.alpha_tmp = calculs.alpha;
		raycasting_vertical(win, player, &calculs);
		calculs.alpha += calculs.dangle;
		if (calculs.alpha > _2_PI)
			calculs.alpha -= _2_PI;
	}

	// t_plan		plan = (t_plan){-2, -6, 2, -34};
	// t_fdot_3d	collision;
	// t_fdot_3d	source = (t_fdot_3d){3, 0, 4};

	// calculs.ray_2 = (t_cartesienne){source.x, source.y, source.z, 6, 2, 4};
	// calculs.alpha_tmp = M_PI_2;
	// calculs.alpha_up = 0.2;
	// set_cartesienne(&calculs, &(calculs.ray_2), source);
	// printf("Collision : %f %f %f\n", collision.x, collision.y, collision.z);
	// printf("Vector : %f %f %f\n", calculs.ray_2.vx, calculs.ray_2.vy, calculs.ray_2.vz);
	return (0);
}

