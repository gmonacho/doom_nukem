#include "doom_nukem.h"

/*
**	Triangulation (avec 2 points) :
**
**	x = (w * w + fdist1 * fdist1 - fdist2 * fdist2) / (2 * w)
**	y = sqrt(fdist1 * fdist1 - x * x)
*/

static void		draw(t_win *win, t_player *player)
{
	t_cartesienne	*ray;
	int		x;
	int		y;

	y = -1;
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
		{
			ray = &(player->rays[y][x]);
			SDL_SetRenderDrawColor(win->rend,	(ray->color >> 16) & 0xFF,\
												(ray->color >> 8) & 0xFF,\
												(ray->color >> 0) & 0xFF,\
												(ray->color >> 24) & 0xFF);
			SDL_RenderDrawPoint(win->rend, x, y);
			ray->dist = -1;
		}
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

	find_coord_plan(&coord_plan, collision, poly->i, poly->j);
	coord_texture = (t_dot){modulo(coord_plan.x * poly->dist12, poly->texture->w),\
							modulo(coord_plan.y * poly->dist13, poly->texture->h)};
	if (coord_texture.x < 0 || coord_texture.y < 0)
	{
		printf("\nSeg fault !\n");
		printf("Collision %f %f %f\n", collision.x, collision.y, collision.z);
		printf("Coord texture/plan %d %d / %f %f\n", coord_texture.x, coord_texture.y, coord_plan.x, coord_plan.y);
		exit(0);
	}
	return (((int *)poly->texture->pixels)[coord_texture.y * poly->texture->w + coord_texture.x]);
}



static void			launch_ray_3d(t_poly *poly, t_cartesienne *ray)
{
	t_fdot_3d		collision;
	double			newdist;

	if (!intersection_plan_line(&collision, poly->equation, ray))
	{
		printf("Parallole !!!\n");
		exit(1);
	}
	if ((newdist = fdist_3d((t_fdot_3d){ray->ox, ray->oy, ray->oz}, collision)) > ray->dist &&\
		ray->dist != -1)
		return ;
	ray->dist = newdist;
	ray->color = find_pixel(poly, collision);
}



static void			square_tracing(t_player *player, t_poly *poly)
{
	int				x;
	int				y;
	
	y = -1;
	while (++y < poly->poly_2d_h)
	{
		x = -1;
		while (++x < poly->poly_2d_w)
		{
			launch_ray_3d(poly, &(player->rays[poly->poly_2d_origin.y + y][poly->poly_2d_origin.x + x]));
		}
	}
}



void		raycasting_3d(t_win *win, t_player *player)
{
	t_poly	*poly;

	surround_walls(win, win->map);
	if (win->view & TEXTURE_VIEW)
	{
		poly = win->map->polys;
		while (poly)
		{
			square_tracing(player, poly);
			poly = poly->next;
		}
		draw(win, player);
	}
	if (win->view & WALL_VIEW)
		draw_projection(win);
	if (win->view & SQUARED_VIEW)
		draw_all_square(win);
	draw_fps();
}
