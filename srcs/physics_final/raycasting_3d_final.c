#include "doom_nukem.h"

/*
**	Triangulation (avec 2 points) :
**
**	x = (w * w + fdist1 * fdist1 - fdist2 * fdist2) / (2 * w)
**	y = sqrt(fdist1 * fdist1 - x * x)
*/

static int			find_coord(t_fdot *coord, t_fdot_3d collision, t_fdot_3d i, t_fdot_3d j)
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



static int			*find_pixel(t_poly *poly, t_fdot_3d collision)
{
	t_fdot			coord_plan;
	t_dot			coord_texture;

	find_coord_plan(&coord_plan, collision, poly->i, poly->j);
	coord_texture = (t_dot){modulo(coord_plan.x * poly->dist12, poly->texture->w),\
							modulo(coord_plan.y * polu->dist13, poly->texture->h)};
	if (coord_texture.x < 0 || coord_texture.y < 0)
	{
		printf("\nSeg fault !\n");
		printf("Collision %f %f %f\n", calculs->closest.x, calculs->closest.y, calculs->closest.z);
		printf("Coord texture/plan %d %d / %f %f\n", coord_texture.x, coord_texture.y, coord_plan.x, coord_plan.y);
		exit(0);
	}
	return ((int *)(poly->texture->pixels)[coord_texture.y * poly->texture->w + coord_texture.x]);
}



static void			launch_ray_3d(t_poly *poly, t_cartesienne *ray)
{
	t_fdot_3d		collision;
	double			newdist;

	if (!intersection_plan_line(&collision, line->equation, ray))
	{
		printf("Parallole !!! : %d\n", ray->x);
		exit(1);
	}
	if (!ray->first &&\
		(newdist = fdist_3d((t_fdot_3d){ray->ox, ray->oy, ray->oz}, collision)) > calculs->dist)
		return ;
	ray->first = 0;
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
			launch_ray_3d(player, poly, &(player->rays[poly->poly_2d_origin.y + y][poly->poly_2d_origin.x + x]));
		}
	}
}



void		raycasting_3d(t_win *win, t_player *player)
{
	t_poly	*poly;

	reset_rays(player);
	surround_walls(win, win->map, player);
	poly = win->map->polys;
	while (poly)
	{
		square_tracing(poly);
		poly = poly->next;
	}
}
