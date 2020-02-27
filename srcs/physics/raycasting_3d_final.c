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
**	- Nouveau 'game()' :				~ 280-400	. 10e-4 (Pareil que raycasting_3d())
*/


static void			*draw(void *param)
{
	t_thread		*thread;
	t_cartesienne	**rays;
	t_cartesienne	*ray;
	int				x;
	int				y;
	int				y_max;

	thread = (t_thread *)param;
	y = (thread->i / (float)N_THREADS) * thread->win->h - 1;
	y_max = ((thread->i + 1) / (float)N_THREADS) * thread->win->h;
	rays = thread->player->rays + y + 1;
	// printf("y = %d\ty_max = %d\trays = %p\n", y, y_max, rays);
	while (++y < y_max)
	{
		ray = *rays;
		x = -1;
		while (++x < thread->win->w)
		{
			thread->win->pixels[y * thread->win->w + x] =\
				process_light(thread->win->map, ray->poly, ray->collision, ray->color);
			ray->poly = NULL;
			ray->color = 0xFF505050;
			ray++;
		}
		rays++;
	}
	// printf("\n\n");
	return (NULL);
}
// static void		draw(t_win *win, t_player *player)
// {
// 	t_cartesienne	**rays;
// 	t_cartesienne	*ray;
// 	int				x;
// 	int				y;

// 	rays = player->rays;
// 	y = -1;
// 	while (++y < win->h)
// 	{
// 		ray = *rays;
// 		x = -1;
// 		while (++x < win->w)
// 		{
// 			win->pixels[y * win->w + x] = ray->poly && win->map->view & LIGHT_VIEW ? process_light(win->map, ray->poly, ray->collision, ray->color) : ray->color;
// 			ray->poly = NULL;
// 			ray->color = 0xFF505050;
// 			ray++;
// 		}
// 		rays++;
// 	}
// 	// printf("\n\n");
// 	SDL_UpdateTexture(win->rend_texture, NULL, win->pixels, win->w * sizeof(Uint32));
// 	SDL_RenderCopy(win->rend, win->rend_texture, NULL, NULL);
// }



int					is_in_poly(t_poly *poly, t_fdot *coord, t_fdot_3d dot)
{
	dot = fdot_3d_sub(dot, poly->dots[0]);
	coord->x = scalar_product(dot, poly->i) / poly->ii;
	coord->y = scalar_product(dot, poly->j) / poly->jj;
	return (coord->x < 0 || coord->x > 1 || coord->y < 0 || coord->y > 1 ||\
			is_null(coord->x - 1, 0.0005) || is_null(coord->y, 0.0005) ? 0 : 1); //????????
}



static int			find_pixel(t_poly *poly, t_fdot_3d collision)
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
	coord_texture = poly->object ? (t_dot){coord_plan.x * poly->texture->w,\
											(1 - coord_plan.y) * poly->texture->h} :\
									(t_dot){modulo(coord_plan.x * poly->i_mag, poly->texture->w),\
											modulo(coord_plan.y * poly->j_mag, poly->texture->h)};
	if (coord_texture.x < 0 || coord_texture.y < 0 || coord_texture.x >= poly->texture->w || coord_texture.y >= poly->texture->h)
	{
		printf("\nSeg fault !\n");
		print_poly(poly, 0);
		print_poly(poly, 1);
		print_poly(poly, 2);
		printf("Object ? %p\n", poly->object);
		printf("box %d %d / %d %d\n", poly->box_x.x, poly->box_x.y, poly->box_y.x, poly->box_y.y);
		printf("Collision %f %f %f\n", collision.x, collision.y, collision.z);
		printf("Coord texture/plan %d %d / %f %f\n", coord_texture.x, coord_texture.y, coord_plan.x, coord_plan.y);
		exit(0);
	}
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

static void			launch_ray_3d(t_poly *poly, t_cartesienne *ray)
{
	t_fdot_3d		collision;
	float			newdist;
	int				color;

	if (!intersection_plan_my_ray(&collision, poly->equation, ray))
		return ;
	newdist = collision.x * collision.x + collision.y * collision.y + collision.z * collision.z;
	if ((!ray->poly || newdist < ray->dist) && (color = find_pixel(poly, collision)) != -1 && (color >> 24) & 0xFF)
	{
		ray->poly = poly;
		ray->color = color;
		ray->collision = collision;
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
	while (poly->visible && ++y < y_max)
		if (0 <= y && y < thread->win->h)
		{
			x = poly->box_x.x;
			while (++x < poly->box_x.y)
				if (0 <= x && x < thread->win->w)
					launch_ray_3d(poly, &(thread->player->rays[y][x]));
		}
	thread->poly = thread->poly->next;
	// printf("i = %d\tNext square\n", thread->i);
	square_tracing(thread);
	return (NULL);
}

void		raycasting_3d(t_win *win, t_map *map)
{
	int		i;
	// clock_t		t1;
	// clock_t		t2;
	// t1 = clock();
	// t2 = clock();
	// printf("Delta time %lf\n", ((float)t2 - t1) / (float)CLOCKS_PER_SEC);

	surround_walls(win, map);
	if (map->view & TEXTURE_VIEW)
	{
		//-------------------
		// printf("Debut raytracing\n");	
		i = -1;
		while (++i < N_THREADS)
		{
			win->threads[i].poly = map->polys;
			pthread_create(&(win->threads[i].thread), NULL, square_tracing, &(win->threads[i]));
		}
		i = -1;
		while (++i < N_THREADS)
			pthread_join(win->threads[i].thread, NULL);
		//-------------------
		
		//-------------------
		i = -1;
		while (++i < N_THREADS)
			pthread_create(&(win->threads[i].thread), NULL, draw, &(win->threads[i]));
		i = -1;
		while (++i < N_THREADS)
			pthread_join(win->threads[i].thread, NULL);
		SDL_UpdateTexture(win->rend_texture, NULL, win->pixels, win->w * sizeof(Uint32));
		SDL_RenderCopy(win->rend, win->rend_texture, NULL, NULL);
		//-------------------
		// draw(win, player);
	}
	if (map->view & WALL_VIEW)
		draw_projection(win);
	if (map->view & BOX_VIEW)
		draw_all_square(win);
	draw_fps();
}
