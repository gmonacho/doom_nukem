#include "doom_nukem.h"
#include <time.h>

/*
**	Triangulation (avec 2 points) :
**
**	x = (w * w + fdist1 * fdist1 - fdist2 * fdist2) / (2 * w)
**	y = sqrt(fdist1 * fdist1 - x * x)
*/

/*
**	- Times of 'raycasting_3d()' :
**	Surround wall :		0.0
**	Raycasting :		~245
**		Square_tracing	0 - 0 - ~78 - 0 - ~33 - ~9 = always ~120
**		Draw			~123
**	Projection draw :	1
**	Square draw :		2
*/

static void		draw(t_win *win, t_player *player)
{
	t_cartesienne	**rays;
	t_cartesienne	*ray;
	int		x;
	int		y;

	rays = player->rays;
	y = -1;
	while (++y < HEIGHT)
	{
		ray = *rays;
		x = -1;
		while (++x < WIDTH)
		{
			SDL_SetRenderDrawColor(win->rend,	(ray->color >> 16) & 0xFF,\
												(ray->color >> 8) & 0xFF,\
												ray->color & 0xFF,\
												(ray->color >> 24) & 0xFF);
			// if (ray->color == -1 || (ray->dist < -0.9 && ray->dist > -1.1))
			// 	SDL_SetRenderDrawColor(win->rend, 50, 50, 50, 255);
			// else
			// {
			// 	SDL_SetRenderDrawColor(win->rend,	(ray->color >> 16) & 0xFF,\
			// 										(ray->color >> 8) & 0xFF,\
			// 										ray->color & 0xFF,\
			// 										(ray->color >> 24) & 0xFF);
			// }
			SDL_RenderDrawPoint(win->rend, x, y);
			ray->dist = -1;
			ray->color = 0x20202020;
			ray++;
		}
		rays++;
	}
}



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
	// clock_t			t1;
	// clock_t			t2;

	// t1 = clock();
	find_coord_plan(&coord_plan, collision, poly->i, poly->j);
	// t2 = clock();
	// printf("find coord %lf\n", ((double)t2 - t1) / (double)CLOCKS_PER_SEC);
	// if (coord_plan.x < 0 || coord_plan.x > 1 || coord_plan.y < 0 || coord_plan.y > 1)
	// {
	// 	// printf("Coo %f %f col %f %f %f\n", coord_plan.x, coord_plan.y, collision.x, collision.y, collision.z);
	// 	// printf("C %f %f\n", coord_plan.x, coord_plan.y);
	// 	return (0x20202020);
	// }
	// printf("chibrax\n");
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
	double			newdist;
	// clock_t			t1;
	// clock_t			t2;

	// t1 = clock();
	if (!intersection_plan_line(&collision, poly->equation, ray))
	{
		printf("Parallole !!!\n");
		exit(1);
	}
	// t2 = clock();
	// printf("In %lf\n", ((double)t2 - t1) / (double)CLOCKS_PER_SEC);

	// t1 = clock();
	newdist = collision.x * collision.x + collision.y * collision.y + collision.z * collision.z;
	// newdist = fdist_3d_squared((t_fdot_3d){ray->ox, ray->oy, ray->oz}, collision);
	// t2 = clock();
	// printf("fdi %lf\n", ((double)t2 - t1) / (double)CLOCKS_PER_SEC);

	if (ray->dist != -1 &&\
		newdist > ray->dist)
		return ;
	// t1 = clock();
	ray->dist = newdist;
	ray->color = find_pixel(poly, collision);
	// printf("%d %f\n", ray->color, ray->dist);
	// t2 = clock();
	// printf("fp %lf\n", ((double)t2 - t1) / (double)CLOCKS_PER_SEC);
}



static void			square_tracing(t_player *player, t_poly *poly)
{
	int				x;
	int				y;

	y = poly->box_y.x;
	while (++y < poly->box_y.y)
	{
		x = poly->box_x.x;
		while (++x < poly->box_x.y)
		{
			launch_ray_3d(poly, &(player->rays[y][x]));
		}
	}
}



void		raycasting_3d(t_win *win, t_player *player)
{
	t_poly	*poly;
	// clock_t			t1;
	// clock_t			t2;

	// printf("1\n");
	// t1 = clock();
	surround_walls(win, win->map);
	
	// printf("2\n");
	if (win->view & TEXTURE_VIEW)
	{
		poly = win->map->polys;
		while (poly)
		{
			square_tracing(player, poly);
			poly = poly->next;
		}
	// t1 = clock();
		draw(win, player);
	// t2 = clock();
	// printf("sur %lf\n", ((double)t2 - t1) / (double)CLOCKS_PER_SEC);
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
