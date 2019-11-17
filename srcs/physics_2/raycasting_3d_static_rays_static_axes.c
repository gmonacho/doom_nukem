#include "doom_nukem.h"

static void			find_coord(t_fdot *coord, t_fdot_3d collision, t_fdot_3d i, t_fdot_3d j)
{
	if (is_null(i.x * j.y - i.y * j.x, 0.005))
		coord->y = (collision.y * i.x - collision.x * i.y) / (i.x * j.y - i.y * j.x);
	else if (is_null(i.y * j.z - i.z * j.y, 0.005))
		coord->y = (collision.z * i.y - collision.y * i.z) / (i.y * j.z - i.z * j.y);
	else if (is_null(i.z * j.x - i.x * j.z, 0.005))
		coord->y = (collision.x * i.z - collision.z * i.x) / (i.z * j.x - i.x * j.z);
	else
	{
		printf("Impossible vecteur unitaire null ???\n");
		exit(0);
	}

	// printf("Coord y %f\n", coord->y);
	if (coord->y < 0)
	{
		printf("Coord negative dans le mur\n");
		exit(0);
	}

	if (!is_null(i.x, 0.005))
		coord->x = (collision.x - j.x * coord->y) / i.x;
	else if (!is_null(i.y, 0.005))
		coord->x = (collision.y - j.y * coord->y) / i.y;
	else if (!is_null(i.z, 0.005))
		coord->x = (collision.z - j.z * coord->y) / i.z;
	else
	{
		printf("Impossible vecteur unitaire i null ???\n");
		exit(0);
	}

	// printf("Coord x %f\n", coord->x);
	if (coord->x < 0)
	{
		printf("Coord negative dans le mur x=%f\n", coord->x);
		exit(0);
	}
}

static void			draw_point(t_win *win, t_calculs *calculs, t_player *player, t_cartesienne *ray)
{
	int				pixel;
	t_fdot			coord_plan;
	t_dot			coord_texture;

	find_coord(&coord_plan, (t_fdot_3d){calculs->closest.x - calculs->collision_wall->origin.x,\
									calculs->closest.y - calculs->collision_wall->origin.y,\
									calculs->closest.z - calculs->collision_wall->origin.z},\
							calculs->collision_wall->i,\
							calculs->collision_wall->j);
	
	coord_texture = (t_dot){(int)modulo(coord_plan.x * fmag(calculs->collision_wall->i), calculs->collision_wall->texture->w),\
							(int)modulo(coord_plan.y * fmag(calculs->collision_wall->j), calculs->collision_wall->texture->h)};
	
	if (coord_texture.x < 0 || coord_texture.y < 0)
	{
		printf("\nSeg fault !\n");
		printf("Type wall : %d\n", calculs->collision_wall->flags);
		printf("Collision %f %f %f\n", calculs->closest.x, calculs->closest.y, calculs->closest.z);
		printf("i / j :\n%f %f %f\n%f %f %f\n",	calculs->collision_wall->i.x, calculs->collision_wall->i.y, calculs->collision_wall->i.z,\
												calculs->collision_wall->j.x, calculs->collision_wall->j.y, calculs->collision_wall->j.z);
		printf("Coord texture/plan %d %d / %f %f\n", coord_texture.x, coord_texture.y, coord_plan.x, coord_plan.y);
		printf("\n");
		exit(0);
	}

	pixel = ((int *)calculs->collision_wall->texture->pixels)[coord_texture.y * calculs->collision_wall->texture->w +\
																coord_texture.x];
	
	SDL_SetRenderDrawColor(win->rend,	(pixel >> 16) & 0xFF,\
										(pixel >> 8) & 0xFF,\
										(pixel >> 0) & 0xFF,\
										(pixel >> 24) & 0xFF);
	SDL_RenderDrawPoint(win->rend, ray->x, ray->y);
	player = NULL;
}

static t_linedef	*launch_ray_3d(t_calculs *calculs, t_cartesienne *ray, t_sector *sector)
{
	t_linedef		*line;
	t_fdot_3d		collision;
	double			tmpdist;

	printf("Ray %f %f %f\n", ray->vx, ray->vy, ray->vz);
	calculs->dist = -1;
	calculs->collision_wall = NULL;
	line = sector->lines;
	while (line)
	{
		if (!intersection_plan_line(&collision, line->equation, ray))
		{
			printf("Parallole !!! : %d\n", ray->x);
			return (NULL);
		}

		printf("----> %d\n", line->flags);
		printf("Line %d %d / %d %d\n", line->p1.x, line->p1.y, line->p2.x, line->p2.y);
		printf("Equation %f %f %f %f\n", line->equation.v.x, line->equation.v.y, line->equation.v.z, line->equation.d);
		printf("%d\n", ((tmpdist = fdist_3d((t_fdot_3d){ray->ox, ray->oy, ray->oz}, collision)) < calculs->dist ||\
						calculs->dist == -1));
		printf("%d\n", sence(*ray, collision));
		printf("%d\n", ((int)collision.x != (int)ray->ox || (int)collision.y != (int)ray->oy || (int)collision.z != (int)ray->oz));

		if (((tmpdist = fdist_3d((t_fdot_3d){ray->ox, ray->oy, ray->oz}, collision)) < calculs->dist ||\
			calculs->dist == -1) &&\
			sence(*ray, collision) &&\
			((int)collision.x != (int)ray->ox || (int)collision.y != (int)ray->oy || (int)collision.z != (int)ray->oz))
 		{
			printf("New collision : %f %f %f\n", collision.x, collision.y, collision.z);
			calculs->collision_wall = line;
			calculs->collision_sector = line->sector;
			calculs->closest = (t_fdot_3d){collision.x, collision.y, collision.z};
			calculs->dist = tmpdist;
		}
		line = line->next;
	}
	if (!calculs->collision_wall)
	{
		printf("COLLISION MUR NULLE IMPOSSIBLE ???????\n");
		exit(0);
	}
	calculs->dist_sum += calculs->dist;
	return (calculs->collision_wall);
}

static t_fdot_3d	begin_launch(t_player *player, t_calculs *calculs, t_cartesienne *ray)
{
	t_cartesienne	ray_tmp;
	t_linedef		*wall;
	t_sector		*sector;

	calculs->dist_sum = 0;
	ray_tmp = (t_cartesienne){0, 0, 0, ray->vx, ray->vy, ray->vz, ray->x, ray->y, NULL};
	sector = player->sector;
	wall = launch_ray_3d(calculs, &ray_tmp, sector);
	while (wall && wall->flags & PORTAL &&
			!(calculs->closest.z < wall->destline->sector->floor_height ||\
			calculs->closest.z > wall->destline->sector->ceil_height) &&\
			calculs->dist_sum < RENDER_DISTANCE)
	{
		ray_tmp.ox = calculs->closest.x;
		ray_tmp.oy = calculs->closest.y;
		ray_tmp.oz = calculs->closest.z;
		teleportation_ray(&ray_tmp, wall, wall->destline, &sector);
		// rotate_ray(&ray_tmp,\
		// 			create_matrice(wall->destline->angle - wall->angle +\
		// 							(sign(wall->p2.x - wall->p1.x) ==\
		// 							sign(wall->destline->p2.x - wall->destline->p1.x) ?\
		// 							0 : M_PI)));
		ray_tmp.vx += wall->destline->equation.v.x - wall->equation.v.x; //Ne marche pas a tout les coups...
		ray_tmp.vy += wall->destline->equation.v.y - wall->equation.v.y;
		ray_tmp.vz += wall->destline->equation.v.z - wall->equation.v.z;
		wall = launch_ray_3d(calculs, &ray_tmp, sector);
	}
	return (calculs->closest);
}



int				raycasting_3d_static_rays_static_axes(t_win *win, t_player *player)
{
	t_calculs	calculs;
	t_cartesienne	*ray;

	calculs.raycast = 1;
	ray = player->rays;
	// printf("Debut moteur\n");
	while (ray)
	{
		begin_launch(player, &calculs, ray);
		draw_point(win, &calculs, player, ray);
		ray = ray->next;
	}
	return (0);
}