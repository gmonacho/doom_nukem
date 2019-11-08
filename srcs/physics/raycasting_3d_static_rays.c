#include "doom_nukem.h"

/*
**	Nouveau raycasting, plus d'angle car cartesienne pre-calcule
*/



// static int			intersection_plan_line(t_fdot_3d *collision, t_plan plan, t_cartesienne *ray)
// {
// 	double			t;
	
// 	if (plan.a * ray.vx + plan.b * ray.vy + plan.c * ray.vz == 0)
// 		return (0);
// 	t = -(plan.a * source.x + plan.b * source.y + plan.c * source.z + plan.d) /\
// 			(double)(plan.a * ray.vx + plan.b * ray.vy + plan.c * ray.vz);
// 	collision->x = ray.vx * t + ray.ox;
// 	collision->y = ray.vy * t + ray.oy;
// 	collision->z = ray.vz * t + ray.oz;
// 	return (1);
// }



// static t_linedef	*test_floor_ceil(t_calculs *calculs, t_cartesienne *ray, t_sector *sector)
// {
// 	t_fdot_3d		collision;
// 	double			tmpdist;

// 	if (!intersection_plan_line(&collision, sector->floor_equation, ray))
// 	{
// 		printf("Parallole !!! : %d\n", ray->x);
// 		return (NULL);
// 	}
// 	if (((tmpdist = fdist_3d((t_fdot_3d){ray->ox, ray->oy, ray->oz}, collision)) < calculs->dist) &&\
// 		sence(ray, collision))
// 	{
// 		calculs->collision_type = 1;
// 		calculs->collision_wall = NULL;
// 		calculs->collision_sector = sector;
// 		calculs->closest_2 = (t_fdot_3d){collision.x, collision.y, collision.z};
// 		calculs->dist = tmpdist;
// 	}

// 	if (!intersection_plan_line(&collision, sector->ceil_equation, ray))
// 	{
// 		printf("Parallole !!! : %d\n", ray->x);
// 		return (NULL);
// 	}
// 	if (((tmpdist = fdist_3d((t_fdot_3d){ray->ox, ray->oy, ray->oz}, collision)) < calculs->dist) &&\
// 		sence(ray, collision))
// 	{
// 		calculs->collision_type = 1;
// 		calculs->collision_wall = NULL;
// 		calculs->collision_sector = sector;
// 		calculs->closest_2 = (t_fdot_3d){collision.x, collision.y, collision.z};
// 		calculs->dist = tmpdist;
// 	}
// 	return (calculs->collision_wall);
// }



// static t_linedef	*launch_ray_3d(t_calculs *calculs, t_cartesienne *ray, t_sector *sector)
// {
// 	t_linedef		*line;
// 	t_fdot_3d		collision;
// 	double			tmpdist;

// 	calculs->dist = -1;
// 	calculs->collision_wall = NULL;
// 	calculs->collision_type = 0;
// 	line = sector->lines;
// 	while (line)
// 	{
// 		if (!intersection_plan_line(&collision, sector->ceil_equation, ray))
// 		{
// 			printf("Parallole !!! : %d\n", ray->x);
// 			return (NULL);
// 		}
// 		if (((tmpdist = fdist_3d((t_fdot_3d){ray->ox, ray->oy, ray->oz}, collision)) < calculs->dist ||\
// 			calculs->dist == -1) &&\
// 			sence(ray, collision) &&\
// 			((int)collision.x != (int)ray->ox || (int)collision.y != (int)ray->oy))
//  		{
// 			calculs->collision_wall = line;
// 			calculs->collision_sector = line->sector;
// 			calculs->closest_2 = (t_fdot_3d){collision.x, collision.y, collision.z};
// 			calculs->dist = tmpdist;
// 			// printf("New coll : %f\n", calculs->dist);
// 		}
// 		line = line->next;
// 	}
// 	test_floor_ceil(calculs, ray, sector);
// 	calculs->dist_sum += calculs->dist;
// 	return (calculs->collision_wall);
// }



// static t_fdot_3d	begin_launch(t_player *player, t_calculs *calculs, t_cartesienne *ray)
// {
// 	t_cartesienne	ray_tmp;
// 	t_linedef		*wall;
// 	t_sector		*sector;

// 	calculs->dist_sum = 0;
// 	sector = player->sector;
// 	ray_tmp = {player->pos_up.x, player->pos_up.y, player->pos_up.z,\
// 				ray->vx, ray->vy, ray->vz, ray->x, ray->y, NULL};
// 	wall = launch_ray_3d(calculs, ray_tmp, sector);
// 	while (wall && wall->flags & PORTAL &&
// 			!(calculs->closest_2.z < wall->destline->sector->floor_height ||\
// 			calculs->closest_2.z > wall->destline->sector->ceil_height) &&\
// 			calculs->dist < RENDER_DISTANCE)
// 	{
// 		// printf("Portal !! %p -> %p\n", wall, wall->destline);
// 		ray_tmp.ox = calculs->closest_2.x;
// 		ray_tmp.oy = calculs->closest_2.y;
// 		ray_tmp.oz = calculs->closest_2.z;
// 		teleportation_ray(&ray_tmp, wall, wall->destline, &sector);
// 		rotate(ray_tmp, create_matrice(wall->destline->angle - wall->angle +\
// 										(sign(wall->p2.x - wall->p1.x) ==\
// 										sign(wall->destline->p2.x - wall->destline->p1.x) ?\
// 										0 : M_PI)));
// 		wall = launch_ray_3d(calculs, ray_tmp, sector);
// 	}
// 	if (!wall)
// 	{
// 		printf("COLLISION MUR NULLE IMPOSSIBLE ???????\n");
// 		exit(0);
// 	}
// 	return (calculs->closest_2);
// }



// int				raycasting_3d(t_win *win, t_player *player)
// {
// 	t_calculs	calculs;
// 	t_cartesienne	*ray;

// 	calculs.raycast = 1;
// 	ray = player->rays;
// 	while (ray)
// 	{
// 		if (ray->x % 2)
// 		{
// 			begin_launch(win, player, &calculs, ray);
// 			draw_point(win, calculs, player, y);
// 		}
// 		ray = ray->next;
// 	}
// 	return (0);
// }