#include "doom_nukem.h"

	// clock_t			t1;
	// clock_t			t2;
	// t1 = clock();
	// t2 = clock();
	// printf("fp %lf\n", ((float)t2 - t1) / (float)CLOCKS_PER_SEC);

/*
**	- Times of 'raycasting_3d()' : ~125-140 . 10e-3
**	Surround wall :		0.0
**	Raycasting :		~120-135
**		nSquare_tracing	0 - 0 - ~78 - 0 - ~33 - ~9 = always ~115-125
**		Draw			~8
**	Projection draw :	1
**	Square draw :		2
**
**	- Time of 'collisions()' : ~0.0 . 10e-3
*/

/*
**	- Nouveau 'collisions_sphere()' :	~ 0.5-1.7	. 10e-4
**	- Nouveau 'raycasting_3d()' :		~ 280-400	. 10e-4
**	- Nouveau 'game()' :				~ 280-400	. 10e-4 (Pareil que raycasing_3d())
*/

static void		draw(t_win *win, t_player *player)
{
	t_cartesienne	**rays;
	t_cartesienne	*ray;
	int				x;
	int				y;

	rays = player->rays;
	y = -1;
	while (++y < win->h)
	{
		ray = *rays;
		x = -1;
		while (++x < win->w)
		{
			// printf("%p\t", ray->poly);
			win->pixels[y * win->w + x] = ray->poly ? ray->color : 0xFF505050;
			ray->poly = NULL;
			ray->color = 0;
			ray++;
		}
		rays++;
	}
	// printf("\n\n");
	SDL_UpdateTexture(win->rend_texture, NULL, win->pixels, win->w * sizeof(Uint32));
	SDL_RenderCopy(win->rend, win->rend_texture, NULL, NULL);
}



int					is_in_poly(t_poly *poly, t_fdot *coord, t_fdot_3d dot)
{
	dot = fdot_3d_sub(dot, poly->dots[0]);
	coord->x = scalar_product(dot, poly->i) / poly->ii;
	coord->y = scalar_product(dot, poly->j) / poly->jj;
	return (coord->x < 0 || coord->x > 1 || coord->y < 0 || coord->y > 1 ? 0 : 1);
}

// static int			is_in_shadow(t_map *map, t_poly *poly_collision, t_fdot_3d light, t_fdot_3d pos, t_fdot_3d ray)
// {
// 	t_poly			*poly;
// 	t_fdot_3d		collision;
// 	t_fdot			coord;

// 	// printf("Light %f, %f, %f\n", light.x, light.y, light.z);
// 	// printf("Ray %f, %f, %f\n", ray.x, ray.y, ray.z);
// 	poly = map->polys;
// 	while (poly)
// 	{
// 		if (poly != poly_collision)
// 		{
// 			// printf("Poly eq. %f %f %f %f\n", poly->equation.v.x, poly->equation.v.y, poly->equation.v.z, poly->equation.d);
// 			if (intersection_plan_ray(&collision, poly->equation,\
// 										(t_cartesienne){light.x, light.y, light.z,\
// 														ray.x, ray.y, ray.z,\
// 														0, NULL, 0, NULL}))
// 			{
// 				// printf("Collision %f, %f, %f\n", collision.x, collision.y, collision.z);
// 				if (is_in_poly(poly, &coord, collision) &&\
// 					is_in_segment(collision, light, pos))
// 				{
// 					// printf("Coord %f %f\n", coord.x, coord.y);
// 					// printf("In poly\n");
// 					return (1);
// 				}
// 				// printf("Collision %d %d\n", coord.x, coord.y);
// 			}
// 			else
// 			{
// 				// printf("Parrallele\n");
// 				return (1);
// 			}
// 		}
// 		poly = poly->next;
// 	}
// 	// printf("Not in shadow\n");
// 	return (0);
// }

static int			process_light(t_map *map, t_poly *poly, t_fdot_3d collision, int color)
{
	t_fdot_3d		ray;
	float			dist;
	float			sc_product;
	float			light_coef;
	t_object		*object;
	int i = -1;

	light_coef = 0;
	object = map->objects;
	while (object)
	{
		if (object->type == LIGHT)
		{
			// printf("Poly collide %f, %f, %f\n", collision.x, collision.y, collision.z);
			//rien faire si scalar < 0
			// printf("Pos %f %f %f\n", object->pos.x, object->pos.y, object->pos.z);
			ray = fdot_3d_sub(collision, object->pos);
			// if (is_in_shadow(map, poly, object->pos, collision, ray))
			// {
			// 	light_coef += 0;
			// }
			// else
			// {
				sc_product = fabs(scalar_product(poly->equation.v, normalize(ray)));
				if ((dist = mag(ray)) < 1000)
					light_coef += sc_product * (1 - dist / 1000) * poly->light_coef * object->data;
			// }
			i++;
		}
		object = object->next;
	}
	// printf("N lights : %d\n", i);
	// exit(0);
	return (color & 0xFF000000 +\
			((int)((color >> 16 & 0xFF) * light_coef) << 16) +\
			((int)((color >> 8 & 0xFF) * light_coef) << 8) +\
			(int)((color & 0xFF) * light_coef));
}



static int			find_pixel(t_map *map, t_poly *poly, t_fdot_3d collision)
{
	t_fdot			coord_plan;
	t_dot			coord_texture;

	if (!is_in_poly(poly, &coord_plan, collision))
		return (-1);
	if (!poly->texture)
	{
		printf("texture null : %p\n", poly->texture);
		exit(0);
	}
	coord_texture = (t_dot){modulo(coord_plan.x * poly->i_mag, poly->texture->w),\
							modulo(coord_plan.y * poly->j_mag, poly->texture->h)};
	if (coord_texture.x < 0 || coord_texture.y < 0 || coord_texture.x > poly->texture->w || coord_texture.y > poly->texture->h)
	{
		printf("\nSeg fault !\n");
		print_poly(poly, 0);
		print_poly(poly, 1);
		print_poly(poly, 2);
		printf("box %d %d / %d %d\n", poly->box_x.x, poly->box_x.y, poly->box_y.x, poly->box_y.y);
		printf("Collision %f %f %f\n", collision.x, collision.y, collision.z);
		printf("Coord texture/plan %d %d / %f %f\n", coord_texture.x, coord_texture.y, coord_plan.x, coord_plan.y);
		exit(0);
	}
	// printf("Collision %f %f %f\n", collision.x, collision.y, collision.z);
	if (map->view & LIGHT_VIEW)
		return (process_light(map, poly, collision, ((int *)poly->texture->pixels)[coord_texture.y * poly->texture->w + coord_texture.x]));
	else
		return (((int *)poly->texture->pixels)[coord_texture.y * poly->texture->w + coord_texture.x]);
}

// static int			average_color(int c1, int c2, int alpha)
// {
// 	return ((alpha + (c2 >> 24) * (255 - alpha) / 255) << 24 |\
// 			((((c1 >> 16) & 0xFF) * alpha) / 255 + ((255 - alpha) * ((c2 >> 16) & 0xFF)) / 255) << 16 |\
// 			((((c1 >> 8) & 0xFF) * alpha) / 255 + ((255 - alpha) * ((c2 >> 8) & 0xFF)) / 255) << 8 |\
// 			((((c1 >> 0) & 0xFF) * alpha) / 255 + ((255 - alpha) * ((c2 >> 0) & 0xFF)) / 255) << 0);

// 	// unsigned char	average;

// 	// average = ((c1 >> 24) + (c2 >> 24) * (255 - c1 >> 24) / 255) << 24;
// 	// average += ((((c1 >> 16) & 0xFF) * alpha) / 255 + ((255 - alpha) * ((c2 >> 26) & 0xFF)) / 255) << 16;
// 	// average += ((((c1 >> 8) & 0xFF) * alpha) / 255 + ((255 - alpha) * ((c2 >> 8) & 0xFF)) / 255) << 8;
// 	// average += ((((c1 >> 0) & 0xFF) * alpha) / 255 + ((255 - alpha) * ((c2 >> 0) & 0xFF)) / 255) << 0;
// 	// return (average);
// }

static void			launch_ray_3d(t_map *map, t_poly *poly, t_cartesienne *ray)
{
	t_fdot_3d		collision;
	float			newdist;
	int				color;

	if (!intersection_plan_my_ray(&collision, poly->equation, ray))
		return ;
	newdist = collision.x * collision.x + collision.y * collision.y + collision.z * collision.z;
	// newdist = sqrt(newdist);

	// color = find_pixel(poly, collision);
	// if (color != -1)
	// {
	// 	if (!ray->poly)
	// 		ray->color = color;
	// 	else if (newdist < ray->dist)
	// 		ray->color = average_color(color, ray->color, color >> 24);
	// 	else
	// 		ray->color = average_color(ray->color, color, ray->color >> 24);
	// 	ray->poly = poly;
	// 	ray->dist = newdist;
	// 	// ray->collision = collision;
	// }
	if (collision.x != collision.x)
	{
		printf("Ray parra %f %f %f\n", ray->vx, ray->vy, ray->vz);
	}
	if ((!ray->poly || newdist < ray->dist) && (color = find_pixel(map, poly, collision)) != -1 && (color >> 24) & 0xFF)
	{
		ray->poly = poly;
		ray->color = color;
		ray->dist = newdist;
	}
}

/*
**	Attention certaine box > 1000 sur x !
*/

// static void			square_tracing(t_win *win, t_player *player, t_poly *poly)
// {
// 	int				x;
// 	int				y;

// 	y = poly->box_y.x;
// 	while (++y < poly->box_y.y)
// 		if (0 <= y && y < win->h)
// 		{
// 			x = poly->box_x.x;
// 			while (++x < poly->box_x.y)
// 			{
// 				if (0 <= x && x < win->w)
// 					launch_ray_3d(poly, &(player->rays[y][x]));
// 			}
// 		}
// }

// void		raycasting_3d(t_win *win, t_player *player)
// {
// 	t_poly	*poly;

// 	surround_walls(win, win->map);
// 	if (win->map->view & TEXTURE_VIEW)
// 	{
// 		poly = win->map->polys;
// 		while (poly)
// 		{
// 			// printf("texture %p\n", poly->texture);
// 			poly->is_slide_ban = 0;
// 			square_tracing(win, player, poly);
// 			poly = poly->next;
// 		}
// 		draw(win, player);
// 	}
// 	if (win->map->view & WALL_VIEW)
// 		draw_projection(win);
// 	if (win->map->view & BOX_VIEW)
// 		draw_all_square(win);
// 	draw_fps();
// }

static void			*square_tracing(void *param)
{
	t_thread		*thread;
	t_poly			*poly;
	int				x;
	int				y;
	int				y_max;

	thread = (t_thread *)param;
	poly = thread->poly;
	if (!poly)
	{
		pthread_exit(NULL);//Opti ?
		return (NULL);
	}
	// printf("Poly %d thread %d\n", poly->index, thread->i);
	y = poly->box_y.x + (poly->box_y.y - poly->box_y.x) * (thread->i / (float)N_THREADS) - 1;
	y_max = poly->box_y.x + (poly->box_y.y - poly->box_y.x) * ((thread->i + 1) / (float)N_THREADS);
	// printf("box ymax - %d %d / %d\n", poly->box_y.x, poly->box_y.y, y_max);
	while (++y < y_max)
		if (0 <= y && y < thread->win->h)
		{
			x = poly->box_x.x;
			while (++x < poly->box_x.y)
			{
				if (0 <= x && x < thread->win->w)
				{
					// while (thread->player->rays[y][x].launch)
						// ;
					// thread->player->rays[y][x].launch = 1;
					launch_ray_3d(thread->map, poly, &(thread->player->rays[y][x]));
					// thread->player->rays[y][x].launch = 0;
				}
			}
		}
	thread->poly = thread->poly->next;
	// printf("i = %d\tNext square\n", thread->i);
	square_tracing(thread);
	return (NULL);
}

void		raycasting_3d(t_win *win, t_player *player)
{
	int		i;

	surround_walls(win, win->map);
	if (win->map->view & TEXTURE_VIEW)
	{

		//-------------------
		// printf("Debut raytracing\n");
		i = -1;
		while (++i < N_THREADS)
		{
			win->threads[i].poly = win->map->polys;
			pthread_create(&(win->threads[i].thread), NULL, square_tracing, &(win->threads[i]));
		}
		i = -1;
		while (++i < N_THREADS)
			pthread_join(win->threads[i].thread, NULL);
		// exit(0);
		//-------------------

		draw(win, player);
	}
	if (win->map->view & WALL_VIEW)
		draw_projection(win);
	if (win->map->view & BOX_VIEW)
		draw_all_square(win);
	draw_fps();
}
