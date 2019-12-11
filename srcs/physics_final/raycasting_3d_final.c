#include "doom_nukem.h"

/*
**	Triangulation (avec 2 points) :
**
**	x = (w * w + fdist1 * fdist1 - fdist2 * fdist2) / (2 * w)
**	y = sqrt(fdist1 * fdist1 - x * x)
*/

/*
**	- Times of 'raycasting_3d()' : ~120-140 . 10e-3
**	Surround wall :		0.0
**	Raycasting :		~120-140
**		nSquare_tracing	0 - 0 - ~78 - 0 - ~33 - ~9 = always ~115-125
**		Draw			~8
**	Projection draw :	1
**	Square draw :		2
**
*/

// if (!(sdl->renderer = SDL_CreateRenderer(sdl->win, -1, SDL_RENDERER_SOFTWARE)))
//         printf("Error creating renderer.\n");

static void		draw(t_win *win, t_player *player)
{
	// Uint32		pixels[HEIGHT * WIDTH];
	// uint32_t	*pixels = (Uint32 *)malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
	// SDL_Texture	*texture;
	t_cartesienne	**rays;
	t_cartesienne	*ray;
	int			x;
	int			y;

	rays = player->rays;
	y = -1;
	while (++y < HEIGHT)
	{
		ray = *rays;
		x = -1;
		while (++x < WIDTH)
		{
			// pixels[y * WIDTH + x] =	(((ray->color >> 16) & 0xFF) << 24) |\
			// 							(((ray->color >> 8) & 0xFF) << 16) |\
			// 							(((ray->color >> 0) & 0xFF) << 8) |\
			// 							(((ray->color >> 24) & 0xFF) << 0);
			win->pixels[y * WIDTH + x] = ray->color;
			ray->dist = -1;
			ray->color = 0x505050FF;
			ray++;
		}
		rays++;
	}
	SDL_UpdateTexture(win->rend_texture, NULL, win->pixels, WIDTH * sizeof(uint32_t));
	SDL_RenderCopy(win->rend, win->rend_texture, NULL, NULL);
	// SDL_RenderPresent(win->rend);
}
// static void		draw(t_win *win, t_player *player)
// {
// 	t_cartesienne	**rays;
// 	t_cartesienne	*ray;
// 	int		x;
// 	int		y;

// 	rays = player->rays;
// 	y = -1;
// 	while (++y < HEIGHT)
// 	{
// 		ray = *rays;
// 		x = -1;
// 		while (++x < WIDTH)
// 		{
// 			SDL_SetRenderDrawColor(win->rend,	(ray->color >> 16) & 0xFF,\
// 												(ray->color >> 8) & 0xFF,\
// 												ray->color & 0xFF,\
// 												(ray->color >> 24) & 0xFF);
// 			// if (ray->color == -1 || (ray->dist < -0.9 && ray->dist > -1.1))
// 			// 	SDL_SetRenderDrawColor(win->rend, 50, 50, 50, 255);
// 			// else
// 			// {
// 			// 	SDL_SetRenderDrawColor(win->rend,	(ray->color >> 16) & 0xFF,\
// 			// 										(ray->color >> 8) & 0xFF,\
// 			// 										ray->color & 0xFF,\
// 			// 										(ray->color >> 24) & 0xFF);
// 			// }
// 			SDL_RenderDrawPoint(win->rend, x, y);
// 			ray->dist = -1;
// 			ray->color = 0x20202020;
// 			ray++;
// 		}
// 		rays++;
// 	}
// }



static int			find_coord_plan(t_fdot *coord, t_fdot_3d collision, t_fdot_3d i, t_fdot_3d j)
{
	double			denominateur;

	if (!is_null((denominateur = i.x * j.y - i.y * j.x), 0.005))
		coord->y = (collision.y * i.x - collision.x * i.y) / denominateur;
	else if (!is_null((denominateur = i.y * j.z - i.z * j.y), 0.005))
		coord->y = (collision.z * i.y - collision.y * i.z) / denominateur;
	else if (!is_null((denominateur = i.z * j.x - i.x * j.z), 0.005))
		coord->y = (collision.x * i.z - collision.z * i.x) / denominateur;
	else
	{
		printf("Impossible vecteur unitaire null ???\n");
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
	return (1);
}



static int			find_pixel(t_poly *poly, t_fdot_3d collision)
{
	t_fdot			coord_plan;
	t_dot			coord_texture;

	find_coord_plan(&coord_plan, collision, poly->i, poly->j);
	// printf("find coord %lf\n", ((double)t2 - t1) / (double)CLOCKS_PER_SEC);
	// if (coord_plan.x < 0 || coord_plan.x > 1 || coord_plan.y < 0 || coord_plan.y > 1)
	// {
	// 	// printf("Coo %f %f col %f %f %f\n", coord_plan.x, coord_plan.y, collision.x, collision.y, collision.z);
	// 	// printf("C %f %f\n", coord_plan.x, coord_plan.y);
	// 	return (0x20202020);
	// }
	coord_texture = (t_dot){modulo(coord_plan.x * poly->dist12, poly->texture->w),\
							modulo(coord_plan.y * poly->dist14, poly->texture->h)};

	
	coord_texture = (t_dot){modulo(coord_plan.x * poly->dist12, poly->texture->w),\
							modulo(coord_plan.y * poly->dist14, poly->texture->h)};
	
	// if (coord_texture.x < 0 || coord_texture.y < 0)
	// {
	// 	printf("\nSeg fault !\n");
	// 	printf("Collision %f %f %f\n", collision.x, collision.y, collision.z);
	// 	printf("Coord texture/plan %d %d / %f %f\n", coord_texture.x, coord_texture.y, coord_plan.x, coord_plan.y);
	// 	exit(0);
	// }
	return (((int *)poly->texture->pixels)[coord_texture.y * poly->texture->w + coord_texture.x]);
}



static void			launch_ray_3d(t_poly *poly, t_cartesienne *ray)
{
	t_fdot_3d		collision;
	// double			newdist;
	// clock_t			t1;
	// clock_t			t2;

	if (!intersection_plan_line(&collision, poly->equation, ray))
	{
		printf("Parallole !!!\n");
		exit(1);
	}
	ray->color = find_pixel(poly, collision);
	// t1 = clock();
	// t2 = clock();
	// printf("fp %lf\n", ((double)t2 - t1) / (double)CLOCKS_PER_SEC);
}
// static void			launch_ray_3d(t_poly *poly, t_cartesienne *ray)
// {
// 	t_fdot_3d		collision;
// 	double			newdist;
// 	// clock_t			t1;
// 	// clock_t			t2;

// 	if (!intersection_plan_line(&collision, poly->equation, ray))
// 	{
// 		printf("Parallole !!!\n");
// 		exit(1);
// 	}
// 	newdist = collision.x * collision.x + collision.y * collision.y + collision.z * collision.z;

// 	if (ray->dist != -1 &&\
// 		newdist > ray->dist)
// 		return ;
// 	ray->dist = newdist;
// 	ray->color = find_pixel(poly, collision);
// 	// t1 = clock();
// 	// t2 = clock();
// 	// printf("fp %lf\n", ((double)t2 - t1) / (double)CLOCKS_PER_SEC);
// }



// static void			square_tracing(t_player *player, t_poly *poly)
// {
// 	int				x;
// 	int				y;

// 	y = poly->box_y.x;
// 	while (++y < poly->box_y.y)
// 	{
// 		x = poly->box_x.x;
// 		while (++x < poly->box_x.y)
// 		{
// 			launch_ray_3d(poly, &(player->rays[y][x]));
// 		}
// 	}
// }
static void			square_tracing(t_win *win, t_player *player, t_poly *poly)
{
	t_poly			*poly_ret;
	t_cartesienne	**rays;
	t_cartesienne	*ray;
	int				x;
	int				y;

	poly_ret = NULL;
	rays = player->rays;
	y = 0;
	while (rays)
	{
		ray = *rays;
		x = -1;
		while (++x < win->w)
		{
			if ((poly_ret = inside_poly(poly_ret, poly, x, y)))
				launch_ray_3d(poly_ret, ray);
			ray++;
		}
		y++;
		rays++;
	}
}



void		raycasting_3d(t_win *win, t_player *player)
{
	t_poly	*poly;
	// clock_t			t1;
	// clock_t			t2;

	// printf("1\n");
	surround_walls(win, win->map);
	
	// printf("2\n");
	if (win->view & TEXTURE_VIEW)
	{
	// t1 = clock();

		poly = win->map->polys;
		// while (poly)
		// {
			square_tracing(win, player, poly);
			// poly = poly->next;
		// }

	// t2 = clock();
	// printf("sur %lf\n", ((double)t2 - t1) / (double)CLOCKS_PER_SEC);
		draw(win, player);
	}

	// t1 = clock();
	if (win->view & WALL_VIEW)
		draw_projection(win);

	// t1 = clock();
	if (win->view & SQUARED_VIEW)
		draw_all_square(win);
	// t2 = clock();
	// printf("sur %lf\n", ((double)t2 - t1) / (double)CLOCKS_PER_SEC);
	// printf("3\n");
	draw_fps();
	// exit(0);
}
