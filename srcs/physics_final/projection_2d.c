#include "doom_nukem.h"
#include <time.h>

/*
**
**	Pour chaque poly :
**		On creer un nouveau tab de dot 3d pour ramener tout les dot.y en positif
**		On les projette en 2d
**		On creer un nouveau tab de dot 2d pour ramener la proj dans l'ecran
**		On creer la box
*/
/*
**	Tester pour les 4 cote chaque segments, si un segment est dessus
**	alors new point et suppr lancien (Comme pour ramener les y<0 devant soi)
**	On fais ca 4 fois au lieu de 1
*/

void			draw_projection(t_win *win)
{
	t_poly		*poly;
	int			i;

	poly = win->map->polys;
	SDL_SetRenderDrawColor(win->rend, 0x50, 0xEE, 0x50, 0xFF);
	while (poly)
	{
		i = -1;
		while (++i < poly->n_proj)
			draw_line(win, poly->dots_proj[i], poly->dots_proj[i ? i - 1 : poly->n_proj - 1]);
		poly = poly->next;
	}
}

void			draw_all_square(t_win *win)
{
	t_poly		*poly;

	poly = win->map->polys;
	SDL_SetRenderDrawColor(win->rend, 0x50, 0x50, 0xEE, 0xFF);
	// printf("Debut draw square map : %p\n", poly);
	while (poly)
	{
		// printf("Draw box ...\n");
		draw_line(win, (t_dot){poly->box_x.x, poly->box_y.x}, (t_dot){poly->box_x.y, poly->box_y.x});
		draw_line(win, (t_dot){poly->box_x.x, poly->box_y.x}, (t_dot){poly->box_x.x, poly->box_y.y});
		draw_line(win, (t_dot){poly->box_x.y, poly->box_y.x}, (t_dot){poly->box_x.y, poly->box_y.y});
		draw_line(win, (t_dot){poly->box_x.x, poly->box_y.y}, (t_dot){poly->box_x.y, poly->box_y.y});
		poly = poly->next;
	}
	// printf("Fin draw square\n");
}
















static void			set_box(t_poly *poly, t_dot *box_x, t_dot *box_y, t_dot proj[N_DOTS_POLY * 2])
{
	int				i;

	i = -1;
	while (++i < poly->n_proj)
	{
		if (proj[i].x < box_x->x)
			box_x->x = proj[i].x;
		if (proj[i].x > box_x->y)
			box_x->y = proj[i].x;
		if (proj[i].y < box_y->x)
			box_y->x = proj[i].y;
		if (proj[i].y > box_y->y)
			box_y->y = proj[i].y;
	}
}



static void		set_proj(t_win *win, t_poly *poly)
{
	int			i;

	i = -1;
	while (++i < poly->n_dot)
	{
		poly->dots_proj[i].x = (win->map->player.fov_2 + poly->dots_new[i].y / poly->dots_new[i].x) * win->w_div_fov;
		poly->dots_proj[i].y = (win->map->player.fov_up_2 - poly->dots_new[i].z / poly->dots_new[i].x) * win->h_div_fov;
		// printf("Proj 2d %d %d of 3d dot %f %f %f\n", poly->dots_proj[i].x, poly->dots_proj[i].y, poly->dots_new[i].x, poly->dots_new[i].y , poly->dots_new[i].z);
	}
	poly->n_proj = poly->n_dot;
}



static t_fdot_3d	intersection_axe_y(t_fdot_3d d1, t_fdot_3d d2)
{
	float			t;

	t = (1 - d1.x) / (d2.x - d1.x);
	// printf("t = %f\n", t);
	return ((t_fdot_3d){1,\
						(d2.y - d1.y) * t + d1.y,\
						(d2.z - d1.z) * t + d1.z});
}

static void		create_dot_on_axe_y(t_poly *poly)
{
	int			i;
	int			i2;

	poly->n_dot = 0;
	i = -1;
	while (++i < N_DOTS_POLY)
	{
		// printf("Dot : %zf %f %f\n", poly->dots[i].x, poly->dots[i].y, poly->dots[i].z);
		i2 = i ? i - 1 : N_DOTS_POLY - 1;
		if (poly->dots[i].x > 0)
		{
			if (poly->dots[i2].x < 0)
			{
				poly->dots_new[poly->n_dot++] = intersection_axe_y(poly->dots[i], poly->dots[i2]);
				// printf("Create new dot 3d on X %f %f %f\n", poly->dots_new[poly->n_dot - 1].x, poly->dots_new[poly->n_dot - 1].y, poly->dots_new[poly->n_dot - 1].z);
			}
			poly->dots_new[poly->n_dot++] = poly->dots[i];
		}
		else if (poly->dots[i2].x > 0)
		{
			poly->dots_new[poly->n_dot++] = intersection_axe_y(poly->dots[i], poly->dots[i2]);
			// printf("Create new dot 3d on X %f %f %f\n", poly->dots_new[poly->n_dot - 1].x, poly->dots_new[poly->n_dot - 1].y, poly->dots_new[poly->n_dot - 1].z);
		}
		// printf("index %d %d\n", poly->n_dot, i);
	}
	// printf("\n");
}

//BOX SORTENT  DE LECRAN WTFF ??

void			surround_walls(t_win *win, t_map *map)
{
	t_poly		*poly;
	// clock_t			t1;
	// clock_t			t2;

	poly = map->polys;
	// printf("\n\nMap polys %p\n", map->polys);
	while (poly)
	{
		poly->box_x = (t_dot){win->w, 0};
		poly->box_y = (t_dot){win->h, 0};
		poly->is_slide_ban = 0;
		if (poly->visible)
		{
			create_dot_on_axe_y(poly);
			set_proj(win, poly);
			poly_reduction(win, poly);
			set_box(poly, &(poly->box_x), &(poly->box_y), poly->dots_proj);
			// printf("BOX xy %d %d / %d %d\n", poly->box_x.x, poly->box_x.y, poly->box_y.x, poly->box_y.y);
		}
		poly = poly->next;
	}
	// printf("\n\n");
}
