// #include "doom_nukem.h"

// /*
// **	Nouveau raycasting, plus d'angle car cartesienne pre-calcule
// */

// static void			draw_point(t_win *win, t_calculs *calculs, t_player *player, int x, int y)
// {
// 	int				pixel;
// 	int				x_texture;
// 	int				y_texture;

// 	if (calculs->collision_type == 0)
// 	{
// 		x_texture = modulo(	fdist(	(t_fdot){calculs->collision_wall->p1.x,\
// 											calculs->collision_wall->p1.y},
// 									(t_fdot){calculs->closest_2.x,\
// 											calculs->closest_2.y}),
// 							calculs->collision_wall->texture->w);
// 		y_texture = modulo(calculs->collision_wall->sector->ceil_height - calculs->closest_2.z,\
// 				   			calculs->collision_wall->texture->h);
// 	}
// 	else
// 	{

// 		x_texture =	modulo(calculs->closest_2.x, calculs->collision_type == 1 ? calculs->collision_sector->floor_texture->w : calculs->collision_sector->ceil_texture->w);
// 		y_texture =	modulo(calculs->closest_2.y, calculs->collision_type == 1 ? calculs->collision_sector->floor_texture->h : calculs->collision_sector->ceil_texture->h);
// 	}

// 	// if (y_texture < 0 || x_texture < 0)
// 	// {
// 	// 	printf("Coll type : %d\n", calculs->collision_type);
// 	// 	printf("Seg fault x y %d %d\tColl %f %f %f\twh %d %d\n", x_texture, y_texture, calculs->closest_2.x, calculs->closest_2.y, calculs->closest_2.z, player->sector->ceil_texture->w, player->sector->ceil_texture->h);
// 	// 	printf("Source %f %f %f\n", player->pos_up.x, player->pos_up.y, player->pos_up.z);
// 	// 	printf("Vector %f %f %f\n", calculs->ray_2.vx, calculs->ray_2.vy, calculs->ray_2.vz);
// 	// }
// 	if (calculs->collision_type == 0)
// 		pixel = ((int *)calculs->collision_wall->texture->pixels)[y_texture * calculs->collision_wall->texture->w + x_texture];
// 	else if (calculs->collision_type == 1)
// 		pixel = ((int *)calculs->collision_sector->floor_texture->pixels)[y_texture * calculs->collision_sector->floor_texture->w + x_texture];
// 	else
// 		pixel = ((int *)calculs->collision_sector->ceil_texture->pixels)[y_texture * calculs->collision_sector->ceil_texture->w + x_texture];

// 	SDL_SetRenderDrawColor(win->rend,	(pixel >> 16) & 0xFF,\
// 										(pixel >> 8) & 0xFF,\
// 										(pixel >> 0) & 0xFF,\
// 										(pixel >> 24) & 0xFF);
// 	SDL_RenderDrawPoint(win->rend, x, y);
// 	player = NULL;
// }






// static t_linedef	*test_floor_ceil(t_calculs *calculs, t_cartesienne *ray, t_sector *sector)
// {
// 	t_fdot_3d		collision;
// 	double			tmpdist;

// 	if (!intersection_plan_line_static(&collision, sector->floor_equation, ray))
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

// 	if (!intersection_plan_line_static(&collision, sector->ceil_equation, ray))
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
// 		if (!intersection_plan_line_static(&collision, sector->ceil_equation, ray))
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




// int				raycasting_3d_static_rays(t_win *win, t_player *player)
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
// 			draw_point(win, calculs, player, ray->x, ray->y);
// 		}
// 		ray = ray->next;
// 	}
// 	return (0);
// }
