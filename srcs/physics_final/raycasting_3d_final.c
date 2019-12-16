#include "doom_nukem.h"

/*
**	Triangulation (avec 2 points, seulement si repere othonorme) :
**
**	x = (w * w + fdist1 * fdist1 - fdist2 * fdist2) / (2 * w)
**	y = sqrt(fdist1 * fdist1 - x * x)
*/

/*
**	- Times of 'raycasting_3d()' : ~125-140 . 10e-3
**	Surround wall :		0.0
**	Raycasting :		~120-135
**		nSquare_tracing	0 - 0 - ~78 - 0 - ~33 - ~9 = always ~115-125
**		Draw			~8
**	Projection draw :	1
**	Square draw :		2
**
*/



static void		draw(t_win *win, t_player *player)
{
	// Uint32		pixels[HEIGHT * WIDTH];
	// uint32_t		*pixels = (Uint32 *)malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
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
			if (!ray->poly)
				ray->color = 0xFF505050;
			SDL_SetRenderDrawColor(win->rend, ((ray->color >> 16) & 0xFF),
											((ray->color >> 8) & 0xFF),
											((ray->color >> 0) & 0xFF),
			 								((ray->color >> 24) & 0xFF));
			SDL_RenderDrawPoint(win->rend, x, y);

			//win->pixels[y * WIDTH + x] = ray->poly ? ray->color :\
			//										0xFF505050;
			ray->poly = NULL;
			ray++;
		}
		rays++;
	}
	// exit(1);
	//SDL_UpdateTexture(win->rend_texture, NULL, win->pixels, WIDTH * sizeof(uint32_t));
	//SDL_RenderCopy(win->rend, win->rend_texture, NULL, NULL);
	//SDL_RenderPresent(win->rend);
	
	
	// y = -1;
	// while (++y < HEIGHT)
	// {
	// 	x = -1;
	// 	while (++x < WIDTH)
	// 		;//printf("%x\n", win->pixels[y * WIDTH + x]);
	// }
}



static int			find_coord_plan(t_fdot *coord, t_fdot_3d dot, t_fdot_3d i, t_fdot_3d j)
{
	float			denom;
	float			uu;
	float			vv;
	float			uv;
	float			pu;
	float			pv;

	uu = i.x * i.x + i.y * i.y + i.z * i.z;
	vv = j.x * j.x + j.y * j.y + j.z * j.z;
	uv = i.x * j.x + i.y * j.y + i.z * j.z;
	pu = dot.x * i.x + dot.y * i.y + dot.z * i.z;
	pv = dot.x * j.x + dot.y * j.y + dot.z * j.z;
	if (!(denom = uv * uv - uu * vv))
		return (0);
	denom = 1 / denom;
	coord->x = (vv * -pu - uv * -pv) * denom;
	coord->y = (uu * -pv - uv * -pu) * denom;

	// if (!is_null((denominateur = i.x * j.y - i.y * j.x), 0.005))
	// 	coord->y = (collision.y * i.x - collision.x * i.y) / denominateur;
	// else if (!is_null((denominateur = i.y * j.z - i.z * j.y), 0.005))
	// 	coord->y = (collision.z * i.y - collision.y * i.z) / denominateur;
	// else if (!is_null((denominateur = i.z * j.x - i.x * j.z), 0.005))
	// 	coord->y = (collision.x * i.z - collision.z * i.x) / denominateur;
	// else
	// {
	// 	printf("Impossible vecteur unitaire j null ??? %f %f %f\n", j.x, j.y, j.z);
	// 	exit(0);
	// }

	// if (!is_null(i.x, 0.005))
	// 	coord->x = (collision.x - j.x * coord->y) / i.x;
	// else if (!is_null(i.y, 0.005))
	// 	coord->x = (collision.y - j.y * coord->y) / i.y;
	// else if (!is_null(i.z, 0.005))
	// 	coord->x = (collision.z - j.z * coord->y) / i.z;
	// else
	// {
	// 	printf("Impossible vecteur unitaire i null ??? %f %f %f\n", i.x, i.y, i.z);
	// 	exit(0);
	// }
	return (1);
}

static int			find_pixel(t_poly *poly, t_fdot_3d collision)
{
	t_fdot			coord_plan;
	t_dot			coord_texture;

	// find_coord_plan(&coord_plan, collision, poly->i, poly->j);
	find_coord_plan(&coord_plan, (t_fdot_3d){	collision.x - poly->dots[0].x,\
												collision.y - poly->dots[0].y,\
												collision.z - poly->dots[0].z}, poly->i, poly->j);
	// printf("find coord %lf\n", ((float)t2 - t1) / (float)CLOCKS_PER_SEC);
	// if (coord_plan.x < 0 || coord_plan.x > 1 || coord_plan.y < 0 || coord_plan.y > 1)
	// {
	// 	printf("Coo %f %f col %f %f %f\n", coord_plan.x, coord_plan.y, collision.x, collision.y, collision.z);
	// 	printf("C %f %f\n\n", coord_plan.x, coord_plan.y);
	// 	return (0x505050FF);
	// }

	if (coord_plan.x < 0 || coord_plan.x > 1 || coord_plan.y < 0 || coord_plan.y > 1)
	{
		// printf("Coord ext %f %f\n", coord_plan.x, coord_plan.y);
		return (-1);
	}

	coord_texture = (t_dot){modulo(coord_plan.x * poly->dist12, poly->texture->w),\
							modulo(coord_plan.y * poly->dist14, poly->texture->h)};

	
	coord_texture = (t_dot){modulo(coord_plan.x * poly->dist12, poly->texture->w),\
							modulo(coord_plan.y * poly->dist14, poly->texture->h)};
	if (coord_texture.x < 0 || coord_texture.y < 0 || coord_texture.x > poly->texture->w || coord_texture.y > poly->texture->h)
	{
		printf("\nSeg fault !\n");
		printf("Collision %f %f %f\n", collision.x, collision.y, collision.z);
		printf("Coord texture/plan %d %d / %f %f\n", coord_texture.x, coord_texture.y, coord_plan.x, coord_plan.y);
		exit(0);
	}
	// printf("coord %d %d\n", coord_texture.x, coord_texture.y);
	// printf("color %x\n", ((int *)poly->texture->pixels)[coord_texture.y * poly->texture->w + coord_texture.x]);
	// return (0xFFFF0000);
	return (((int *)poly->texture->pixels)[coord_texture.y * poly->texture->w + coord_texture.x]);
}

static void			launch_ray_3d(t_poly *poly, t_cartesienne *ray)
{
	t_fdot_3d		collision;
	float			newdist;
	int				color;
	// clock_t			t1;
	// clock_t			t2;

	if (!intersection_plan_line(&collision, poly->equation, ray))
		return ;
	newdist = collision.x * collision.x + collision.y * collision.y + collision.z * collision.z;
	// newdist = sqrt(newdist);

	if ((!ray->poly || newdist < ray->dist) && (color = find_pixel(poly, collision)) != -1)
	{
		ray->poly = poly;
		ray->color = color;
		ray->dist = newdist;
		ray->collision = collision;
	}
	// printf("Ntm %p\n", poly);
	// t1 = clock();
	// t2 = clock();
	// printf("fp %lf\n", ((float)t2 - t1) / (float)CLOCKS_PER_SEC);
}



static void			square_tracing(t_player *player, t_poly *poly)
{
	int				x;
	int				y;
	// clock_t			t1;
	// clock_t			t2;

	// t1 = clock();
	y = poly->box_y.x;
	while (++y < poly->box_y.y)
	{
		x = poly->box_x.x;
		while (++x < poly->box_x.y)
		{
			launch_ray_3d(poly, &(player->rays[y][x]));
			// printf("%f %f %f\n", player->rays[y][x].vx, player->rays[y][x].vy, player->rays[y][x].vz);
		}
	}
	// t2 = clock();
	// printf("time calcul %lf\n", ((float)t2 - t1) / (float)CLOCKS_PER_SEC);
}



void		raycasting_3d(t_win *win, t_player *player)
{
	t_poly	*poly;
	// clock_t			t1;
	// clock_t			t2;

	// t1 = clock();
	// t2 = clock();
	// printf("time %lf\n", ((float)t2 - t1) / (float)CLOCKS_PER_SEC);
	// printf("1\n");
	surround_walls(win, win->map);

	// printf("2\n");
	if (win->view & TEXTURE_VIEW)
	{
		poly = win->map->polys;
		while (poly)
		{
			// printf("MAIS NIQUE TA MERE %p\n", poly);
			square_tracing(player, poly);
			poly = poly->next;
		}
		draw(win, player);
	}

	// // t1 = clock();
	if (win->view & WALL_VIEW)
		draw_projection(win);

	// // t1 = clock();
	if (win->view & BOX_VIEW)
		draw_all_square(win);
	// t2 = clock();
	// printf("sur %lf\n", ((float)t2 - t1) / (float)CLOCKS_PER_SEC);
	draw_fps();
	// SDL_RenderPresent(win->rend);
	// exit(0);
}
