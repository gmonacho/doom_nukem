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
			draw_line(win, poly->dots_proj[i], poly->dots_proj[i ? i - 1 : N_DOTS_POLY - 1]);
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
		printf("poly ...\n");
		draw_line(win, (t_dot){poly->box_x.x, poly->box_y.x}, (t_dot){poly->box_x.y, poly->box_y.x});
		draw_line(win, (t_dot){poly->box_x.x, poly->box_y.x}, (t_dot){poly->box_x.x, poly->box_y.y});
		draw_line(win, (t_dot){poly->box_x.y, poly->box_y.x}, (t_dot){poly->box_x.y, poly->box_y.y});
		draw_line(win, (t_dot){poly->box_x.x, poly->box_y.y}, (t_dot){poly->box_x.y, poly->box_y.y});
		poly = poly->next;
	}
	// printf("Fin draw square\n");
}













static t_fdot_3d	intersection_axe_y(t_fdot_3d d1, t_fdot_3d d2)
{
	double			t;

	t = (1 - d1.x) / (d2.x - d1.x);
	printf("t = %f\n", t);
	return ((t_fdot_3d){1,\
						(d2.y - d1.y) * t + d1.y,\
						(d2.z - d1.z) * t + d1.z});
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

static int		intersection(t_win *win, t_dot *collision, t_dot d1, t_dot d2, int choice)
{
	double		a;
	double		b;
	double		num;
	double		denom;
	double		result;

	printf("Collisino : %p\n", collision);
	num = d1.y - d2.y;
	if (!(denom = d1.x - d2.x))
	{
		return ((0 < d1.x && d1.x < win->w &&\
				((d1.y < 0 && 0 < d2.y) ||\
				(d2.y < 0 && 0 < d1.y))) ? 1 : 0);
	}
	a = num / denom;
	b = d1.y - a * d1.x;
	printf("Line inter screen : y = %f * x + %f\n", a, b);
	result = -1;
	if (choice == 0)
	{
		if (num)
			result = -b / a;
		*collision = (t_dot){result, 0};
		printf("Collision haut %d %d\n", collision->x, collision->y);
		return (0 < result && result < win->w ? 1 : 0);
	}
	else if (choice == 1)
	{
		if (num)
			result = a * win->w + b;
		*collision = (t_dot){win->w, result};
		printf("Collision droite %d %d\n", collision->x, collision->y);
		return (0 < result && result < win->h ? 1 : 0);
	}
	else if (choice == 2)
	{
		if (num)
			result = (win->h - b) / a;
		*collision = (t_dot){result, win->h};
		printf("Collision bas %d %d\n", collision->x, collision->y);
		return (0 < result && result < win->w ? 1 : 0);
	}
	else if (choice == 3)
	{
		if (num)
			result = b;
		*collision = (t_dot){0, result};
		printf("Collision gauche %d %d\n", collision->x, collision->y);
		return (0 < result && result < win->h ? 1 : 0);
	}
	else
		return (0);
}

// static void		cpy_proj(t_dot **proj[N_DOTS_POLY * 2], t_dot new_proj[N_DOTS_POLY * 2], int *i, int new_i)
// {
// 	int			copy;

// }

static void		poly_reduction(t_win *win, t_poly *poly)
{
	t_dot		dot;
	t_dot		new_proj[N_DOTS_POLY * 2];
	int			i;

	poly->n_proj = 0;
	i = -1;
	while (++i < poly->n_dot)
	{
		dot = poly->dots_proj[i];
		if (intersection(win, &(new_proj[poly->n_proj]), dot, poly->dots_proj[i ? i - 1 : poly->n_dot - 1], 0))
			poly->n_proj++;
		if (intersection(win, &(new_proj[poly->n_proj]), dot, poly->dots_proj[i ? i - 1 : poly->n_dot - 1], 1))
			poly->n_proj++;
		if (intersection(win, &(new_proj[poly->n_proj]), dot, poly->dots_proj[i ? i - 1 : poly->n_dot - 1], 2))
			poly->n_proj++;
		if (intersection(win, &(new_proj[poly->n_proj]), dot, poly->dots_proj[i ? i - 1 : poly->n_dot - 1], 3))
			poly->n_proj++;
		if (dot.x > 0 && dot.x < win->w &&\
			dot.y > 0 && dot.y < win->h)
			new_proj[poly->n_proj++] = dot;
	}
	printf("Nbr new intersection screen %d\n", poly->n_proj - i);
	i = -1;
	while (++i < poly->n_proj)
		poly->dots_proj[i] = new_proj[i];
}

static void		set_proj(t_win *win, t_poly *poly)
{
	int			i;

	i = -1;
	while (++i < poly->n_dot)
	{
		poly->dots_proj[i].x = (win->map->player.fov_2 + poly->dots_new[i].y / poly->dots_new[i].x) * win->w / win->map->player.fov;
		poly->dots_proj[i].y = (win->map->player.fov_up_2 - poly->dots_new[i].z / poly->dots_new[i].x) * win->h / win->map->player.fov_up;
		printf("Proj 2d %d %d of 3d dot %f %f %f\n", poly->dots_proj[i].x, poly->dots_proj[i].y, poly->dots_new[i].x, poly->dots_new[i].y , poly->dots_new[i].z);
	}
	poly->n_proj = poly->n_dot;
}

static void		create_dot_on_axe_y(t_poly *poly)
{
	int			i;
	int			i2;

	poly->n_dot = 0;
	i = -1;
	while (++i < N_DOTS_POLY)
	{
		i2 = i ? i - 1 : N_DOTS_POLY - 1;
		if (poly->dots[i].x > 0)
		{
			if (poly->dots[i2].x < 0)
			{
				poly->dots_new[poly->n_dot++] = intersection_axe_y(poly->dots[i], poly->dots[i2]);
				printf("Create new dot 3d on X %f %f %f\n", poly->dots[poly->n_dot - 1].x, poly->dots[poly->n_dot - 1].y, poly->dots[poly->n_dot - 1].z);
			}
			poly->dots_new[poly->n_dot++] = poly->dots[i];
		}
		else if (poly->dots[i2].x > 0)
		{
			poly->dots_new[poly->n_dot++] = intersection_axe_y(poly->dots[i], poly->dots[i2]);
			printf("Create new dot 3d on X %f %f %f\n", poly->dots[poly->n_dot - 1].x, poly->dots[poly->n_dot - 1].y, poly->dots[poly->n_dot - 1].z);
		}
		printf("index %d %d\n", poly->n_dot, i);
	}
}

void			surround_walls(t_win *win, t_map *map)
{
	t_poly		*poly;

	poly = map->polys;
	printf("Map polys %p\n", map->polys);
	while (poly)
	{
		printf("Surround poly\n");
		poly->box_x = (t_dot){win->w, 0};
		poly->box_y = (t_dot){win->h, 0};

		create_dot_on_axe_y(poly);
		set_proj(win, poly);
		poly_reduction(win, poly);
		set_box(poly, &(poly->box_x), &(poly->box_y), poly->dots_proj);

		printf("box xy %d %d / %d %d\n", poly->box_x.x, poly->box_x.y, poly->box_y.x, poly->box_y.y);
		poly = poly->next;
	}
}
