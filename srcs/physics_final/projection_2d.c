#include "doom_nukem.h"

/*
**	tan(a) = dx / dy;
**	On oublie le tan pour annuler le fish-eyes
**
**	Pour chaque poly :
**		On creer un nouveau tab de dot 3d pour ramener tout les dot.y en positif
**		On les projette en 2d
**		On creer un nouveau tab de dot 2d pour ramener la proj dans l'ecran
**		On creer la box
*/

/*
**	ATTENTION, il faut prendre en compte le bas et le haut de lecran
**	si les traits passent seulement par les bords gauche et droits !
**
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



// static int		intersection(t_win *win, t_dot *collision, t_dot d1, t_dot d2, int choice)
// {
// 	double		a;
// 	double		b;
// 	double		num;
// 	double		denom;
// 	double		result;

// 	num = d1.y - d2.y;
// 	if (!(denom = d1.x - d2.x))
// 	{
// 		if (choice % 2)
// 			return (0);
// 		result = choice ? win->h : 0;
// 		*collision = (t_dot){d1.x, result};
// 		return ((0 < d1.x && d1.x < win->w &&\
// 				((d1.y < result && result < d2.y) ||\
// 				(d1.y > result && result > d2.y))) ? 1 : 0);
// 	}
// 	a = num / denom;
// 	b = d1.y - a * d1.x;
// 	// printf("Line inter screen : y = %f * x + %f\n", a, b);
// 	if (choice == 0)
// 	{
// 		result = num ? -b / a : -1;
// 		if (result < 0)
// 			result = 0;
// 		else if (result > win->w)
// 			result = win->w;
// 		*collision = (t_dot){result, 0};
// 		printf("Collision haut %d %d", collision->x, collision->y);
// 		return ((d1.x < collision->x && collision->x < d2.x) ||\
// 				(d1.x > collision->x && collision->x > d2.x) ? 1 : 0);
// 				// 0 < result && result < win->w ? 1 : 0);
// 	}
// 	else if (choice == 1)
// 	{
// 		result = a * win->w + b;
// 		if (result < 0)
// 			result = 0;
// 		else if (result > win->h)
// 			result = win->h;
// 		*collision = (t_dot){win->w, result};
// 		printf("Collision droite %d %d / %d %d / %d %d", d1.x, d1.y, collision->x, collision->y, d2.x, d2.y);
// 		return ((d1.x < collision->x && collision->x < d2.x) ||\
// 				(d1.x > collision->x && collision->x > d2.x) ? 1 : 0);
// 				// 0 < result && result < win->h ? 1 : 0);
// 	}
// 	else if (choice == 2)
// 	{
// 		result = num ? (win->h - b) / a : -1;
// 		if (result < 0)
// 			result = 0;
// 		else if (result > win->w)
// 			result = win->w;
// 		*collision = (t_dot){result, win->h};
// 		printf("Collision bas %d %d", collision->x, collision->y);
// 		return ((d1.x < collision->x && collision->x < d2.x) ||\
// 				(d1.x > collision->x && collision->x > d2.x) ? 1 : 0);
// 				// 0 < result && result < win->w ? 1 : 0);
// 	}
// 	else if (choice == 3)
// 	{
// 		result = b;
// 		if (result < 0)
// 			result = 0;
// 		else if (result > win->h)
// 			result = win->h;
// 		*collision = (t_dot){0, result};
// 		printf("Collision gauche %d %d", collision->x, collision->y);
// 		return ((d1.x < collision->x && collision->x < d2.x) ||\
// 				(d1.x > collision->x && collision->x > d2.x) ? 1 : 0);
// 				// 0 < result && result < win->h ? 1 : 0);
// 	}
// 	else
// 		return (0);
// }



static void		set_proj(t_win *win, t_poly *poly)
{
	int			i;

	i = -1;
	while (++i < poly->n_dot)
	{
		poly->dots_proj[i].x = (win->map->player.fov_2 + poly->dots_new[i].y / poly->dots_new[i].x) * win->w / win->map->player.fov;
		poly->dots_proj[i].y = (win->map->player.fov_up_2 - poly->dots_new[i].z / poly->dots_new[i].x) * win->h / win->map->player.fov_up;
		// printf("Proj 2d %d %d of 3d dot %f %f %f\n", poly->dots_proj[i].x, poly->dots_proj[i].y, poly->dots_new[i].x, poly->dots_new[i].y , poly->dots_new[i].z);
	}
	poly->n_proj = poly->n_dot;
}



static t_fdot_3d	intersection_axe_y(t_fdot_3d d1, t_fdot_3d d2)
{
	double			t;

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



void			surround_walls(t_win *win, t_map *map)
{
	t_poly		*poly;

	poly = map->polys;
	// printf("\n\nMap polys %p\n", map->polys);
	while (poly)
	{
		poly->box_x = (t_dot){win->w, 0};
		poly->box_y = (t_dot){win->h, 0};

		print_poly(poly, 0);
		create_dot_on_axe_y(poly);
		print_poly(poly, 1);
		set_proj(win, poly);
		print_poly(poly, 2);
		poly_reduction(win, poly);
		// print_poly(poly, 2);
		set_box(poly, &(poly->box_x), &(poly->box_y), poly->dots_proj);

		printf("BOX xy %d %d / %d %d\n\n", poly->box_x.x, poly->box_x.y, poly->box_y.x, poly->box_y.y);
		poly = poly->next;
	}
	printf("\n\n");
}
