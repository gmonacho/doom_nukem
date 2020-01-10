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
**	- Time of 'collisions()' : ~0.0 . 10e-3
*/

static void		draw(t_win *win, t_player *player)
{
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
			win->pixels[y * WIDTH + x] = ray->poly ? ray->color :\
													0xFF505050;
			ray->poly = NULL;
			ray++;
		}
		rays++;
	}

	// SDL_SetTextureBlendMode(win->red_texture, SDL_BLENDMODE_BLEND);		//Set transparent
	// SDL_SetRenderDrawBlendMode(win->rend, SDL_BLENDMODE_BLEND);
	// SDL_SetRenderTarget(win->rend, win->red_texture);
	// SDL_SetRenderDrawBlendMode(win->rend, SDL_BLENDMODE_NONE);
	
	// SDL_SetRenderTarget(win->rend, NULL);

	SDL_UpdateTexture(win->rend_texture, NULL, win->pixels, WIDTH * sizeof(uint32_t));
	SDL_RenderCopy(win->rend, win->rend_texture, NULL, NULL);
}


void			find_coord_plan(t_poly *poly, t_fdot *coord, t_fdot_3d dot)
{
	coord->x = scalar_product(dot, poly->i) / poly->ii;
	coord->y = scalar_product(dot, poly->j) / poly->jj;
}

// int			find_coord_plan(t_poly *poly, t_fdot *coord, t_fdot_3d dot)
// {
// 	float			div;
// 	float			pi;
// 	float			pj;

// 	pi = dot.x * poly->i.x + dot.y * poly->i.y + dot.z * poly->i.z;
// 	pj = dot.x * poly->j.x + dot.y * poly->j.y + dot.z * poly->j.z;
// 	if (!(div = poly->ijij_iijj))
// 		return (0);
// 	div = 1 / div;
// 	coord->x = (-poly->jj * pi) * div;
// 	coord->y = (-poly->ii * pj) * div;
// 	// coord->x = (-poly->jj * pi + poly->ij * pj) * div;
// 	// coord->y = (-poly->ii * pj + poly->ij * pi) * div;

// 	// float denominateur;

// 	// if (!is_null((denominateur = i.x * j.y - i.y * j.x), 0.005))
// 	// 	coord->y = (dot.y * i.x - dot.x * i.y) / denominateur;
// 	// else if (!is_null((denominateur = i.y * j.z - i.z * j.y), 0.005))
// 	// 	coord->y = (dot.z * i.y - dot.y * i.z) / denominateur;
// 	// else if (!is_null((denominateur = i.z * j.x - i.x * j.z), 0.005))
// 	// 	coord->y = (dot.x * i.z - dot.z * i.x) / denominateur;
// 	// else
// 	// {
// 	// 	printf("Impossible vecteur unitaire j null ??? %f %f %f\n", j.x, j.y, j.z);
// 	// 	exit(0);
// 	// }

// 	// if (!is_null(i.x, 0.005))
// 	// 	coord->x = (dot.x - j.x * coord->y) / i.x;
// 	// else if (!is_null(i.y, 0.005))
// 	// 	coord->x = (dot.y - j.y * coord->y) / i.y;
// 	// else if (!is_null(i.z, 0.005))
// 	// 	coord->x = (dot.z - j.z * coord->y) / i.z;
// 	// else
// 	// {
// 	// 	printf("Impossible vecteur unitaire i null ??? %f %f %f\n", i.x, i.y, i.z);
// 	// 	exit(0);
// 	// }
// 	return (1);
// }

static int			find_pixel(t_poly *poly, t_fdot_3d collision)
{
	t_fdot			coord_plan;
	t_dot			coord_texture;

	// find_coord_plan(poly, &coord_plan, (t_fdot_3d){	collision.x - poly->dots[0].x,\
	// 												collision.y - poly->dots[0].y,\
	// 												collision.z - poly->dots[0].z});
	find_coord_plan(poly, &coord_plan, (t_fdot_3d){	collision.x - poly->dots[0].x,\
													collision.y - poly->dots[0].y,\
													collision.z - poly->dots[0].z});
	// printf("find coord %lf\n", ((float)t2 - t1) / (float)CLOCKS_PER_SEC);
	// if (coord_plan.x < 0 || coord_plan.x > 1 || coord_plan.y < 0 || coord_plan.y > 1)
	// {
	// 	printf("Coo %f %f col %f %f %f\n", coord_plan.x, coord_plan.y, collision.x, collision.y, collision.z);
	// 	printf("C %f %f\n\n", coord_plan.x, coord_plan.y);
	// 	return (0x505050FF);
	// }

	if (coord_plan.x < 0 || coord_plan.x > 1 || coord_plan.y < 0 || coord_plan.y > 1)
	{
		// if (collision.y > 149 && collision.y < 151 && collision.x > 100 && collision.x < 600 && collision.z > 50 && collision.z < 200)
		// {
		// 	printf("Coord ext %f %f\n", coord_plan.x, coord_plan.y);
		// 	printf("Collision %f %f %f\n", collision.x, collision.y, collision.z);
		// 	printf("Collision %f %f %f\n", poly->dots[0].x, poly->dots[0].y, poly->dots[0].z);
		// 	printf("i j %f %f %f / %f %f %f\n", poly->i.x, poly->i.y, poly->i.z, poly->j.x, poly->j.y, poly->j.z);
		// }
		return (-1);
	}

	coord_texture = (t_dot){modulo(coord_plan.x * poly->dist12, poly->texture->w),\
							modulo(coord_plan.y * poly->dist14, poly->texture->h)};

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
	// printf("coord %d %d\n", coord_texture.x, coord_texture.y);
	// printf("color %x\n", ((int *)poly->texture->pixels)[coord_texture.y * poly->texture->w + coord_texture.x]);
	// return (0xFFFF0000);
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
	// clock_t			t1;
	// clock_t			t2;


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
	if ((!ray->poly || newdist < ray->dist) && (color = find_pixel(poly, collision)) != -1)
	{
		ray->poly = poly;
		ray->color = color;
		ray->dist = newdist;
		// ray->collision = collision;
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

/*
**	Attention certaine box > 1000 sur x !
*/

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
