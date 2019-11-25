#include "doom_nukem.h"

/*
**	tan(a) = dx / dy;
**	On oublie le tan pour annuler le fish-eyes
*/

void			draw_projection(t_win *win)
{
	t_poly		*poly;

	poly = win->map->polys;
	while (poly)
	{
		poly->index_proj = 0;
		SDL_SetRenderDrawColor(win->rend, 0x50, 0xEE, 0x50, 0xFF);

		if (poly->d1.x > 0)
		{
			if (poly->d2.x > 0)
				draw_line(win, poly->proj_d1, poly->proj_d2);
			else
				draw_line(win, poly->proj_d1, poly->proj_tmp[poly->index_proj++]);
		}
		else
		{
			if (poly->d2.x > 0)
				draw_line(win, poly->proj_d2, poly->proj_tmp[poly->index_proj++]);
		}

		if (poly->d2.x > 0)
		{
			if (poly->d4.x > 0)
				draw_line(win, poly->proj_d2, poly->proj_d4);
			else
				draw_line(win, poly->proj_d2, poly->proj_tmp[poly->index_proj++]);
		}
		else
		{
			if (poly->d4.x > 0)
				draw_line(win, poly->proj_d4, poly->proj_tmp[poly->index_proj++]);
		}

		if (poly->d4.x > 0)
		{
			if (poly->d3.x > 0)
				draw_line(win, poly->proj_d4, poly->proj_d3);
			else
				draw_line(win, poly->proj_d4, poly->proj_tmp[poly->index_proj++]);
		}
		else
		{
			if (poly->d3.x > 0)
				draw_line(win, poly->proj_d3, poly->proj_tmp[poly->index_proj++]);
		}

		if (poly->d3.x > 0)
		{
			if (poly->d1.x > 0)
				draw_line(win, poly->proj_d3, poly->proj_d1);
			else
				draw_line(win, poly->proj_d3, poly->proj_tmp[poly->index_proj++]);
		}
		else
		{
			if (poly->d1.x > 0)
				draw_line(win, poly->proj_d1, poly->proj_tmp[poly->index_proj++]);
		}
		poly = poly->next;
	}
}

void			draw_all_square(t_win *win)
{
	t_poly		*poly;

	poly = win->map->polys;
	while (poly)
	{
		SDL_SetRenderDrawColor(win->rend, 0x50, 0x50, 0xEE, 0xFF);
		draw_line(win, (t_dot){poly->box_x.x, poly->box_y.x}, (t_dot){poly->box_x.y, poly->box_y.x});
		draw_line(win, (t_dot){poly->box_x.x, poly->box_y.x}, (t_dot){poly->box_x.x, poly->box_y.y});
		draw_line(win, (t_dot){poly->box_x.y, poly->box_y.x}, (t_dot){poly->box_x.y, poly->box_y.y});
		draw_line(win, (t_dot){poly->box_x.x, poly->box_y.y}, (t_dot){poly->box_x.y, poly->box_y.y});
		poly = poly->next;
	}
}













static t_fdot_3d	intersection_axe_x(t_fdot_3d d1, t_fdot_3d d2)
{
	double			t;

	t = (1 - d1.x) / (d2.x - d1.x);
	return ((t_fdot_3d){1,\
						(d2.y - d1.y) * t + d1.y,\
						(d2.z - d1.z) * t + d1.z});
}





static void			set_box(t_win *win, t_dot *box_x, t_dot *box_y, t_fdot_3d dot, t_dot *proj)
{
	if (proj->x < box_x->x)
		box_x->x = proj->x;
	if (proj->x > box_x->y)
		box_x->y = proj->x;
	if (proj->y < box_y->x)
		box_y->x = proj->y;
	if (proj->y > box_y->y)
		box_y->y = proj->y;
}

static int		intersection(t_win *win, t_dot *collision, t_dot d1, t_dot d2, int choice)
{
	double		a;
	double		b;
	double		num;
	double		denom;
	double		result;

	num = d1.y - d2.y;
	if (!(denom = d1.x - d2.x))
	{
		return ((0 < d1.x && d1.x < win->w &&\
				(d1.y < 0 && 0 < d2.y ||\
				d2.y < 0 && 0 < d1.y)) ? 1 : 0);
	}
	a = num / denom;
	b = d1.y - a * d1.x;
	result = -1;
	if (choice == 0)
	{
		if (num)
			result = -b / a;
		*collision = (t_dot){result, 0};
		return (0 < result && result < win->w ? 1 : 0);
	}
	else if (choice == 1)
	{
		if (num)
			result = a * win->w + b;
		*collision = (t_dot){win->w, result};
		return (0 < result && result < win->h ? 1 : 0);
	}
	else if (choice == 2)
	{
		if (num)
			result = (win->h - b) / a;
		*collision = (t_dot){result, win->h};
		return (0 < result && result < win->w ? 1 : 0);
	}
	else if (choice == 3)
	{
		if (num)
			result = b;
		*collision = (t_dot){0, result};
		return (0 < result && result < win->h ? 1 : 0);
	}
	else
		return (0);
}

static void		cpy_proj(t_fdot_3d *proj[N_DOTS_POLY * 2], t_fdot_3d new_proj[N_DOTS_POLY * 2], int *i, int new_i)
{
	int			copy;

	*i = new_i;
	copy = -1;
	while (++copy < N_DOTS_POLY * 2)
		(*proj)[copy] = new_proj[copy];
}

static void		poly_reduction(t_win *win, t_poly *poly)
{
	t_fdot_3d	dot;
	t_fdot_3d	new_proj[N_DOTS_POLY * 2];
	int			i_new;
	int			i;

	i_new = 0;
	i = -1;
	while (++i < N_DOTS_POLY * 2)
	{
		dot = poly->dots_proj[i];
		if (dot.x > 0 && dot.x < win->w &&\
			dot.y > 0 && dot.y < win->h)
			new_proj[i_new++] = dot;
		if (intersection(&(new_proj[i_new]), dot, poly->dots_proj[i ? i - 1 : poly->n_proj - 1], 0))
			i_new++;
		if (intersection(&(new_proj[i_new]), dot, poly->dots_proj[i ? i - 1 : poly->n_proj - 1], 1))
			i_new++;
		if (intersection(&(new_proj[i_new]), dot, poly->dots_proj[i ? i - 1 : poly->n_proj - 1], 2))
			i_new++;
		if (intersection(&(new_proj[i_new]), dot, poly->dots_proj[i ? i - 1 : poly->n_proj - 1], 3))
			i_new++;
	}
	cpy_proj(&poly->dots_proj, new_proj, poly->n_proj, i_new);
}

static void		set_proj(t_win *win, t_poly *poly, t_fdot_3d dot, int i)
{
	if (poly->dots_state[i])
	{
		poly->dots_proj[i]->x = (win->map->player.fov_2 + dot.y / dot.x) * win->w / win->map->player.fov;
		poly->dots_proj[i]->y = (win->map->player.fov_up_2 - dot.z / dot.x) * win->h / win->map->player.fov_up;
	}
}

static void		create_dot_axe_x(t_poly *poly, int d1, int d2)
{
	if (poly->dots[d1].x > 0)
	{
		if (poly->dots[d2].x < 0)
		{
			poly->dots_state[d2] = -1;
			poly->dots_state[poly->n_dot] = d2;
			poly->dots[poly->n_dot++] = intersection_axe_x(d1, d2);
		}
	}
	else if (poly->dots[d2].x > 0)
	{
		poly->dots_state[d1] = -1;
		poly->dots_state[poly->n_dot] = d1;
		poly->dots[poly->n_dot++] = intersection_axe_x(d1, d2);
	}
}

void			surround_walls(t_win *win, t_map *map)
{
	t_poly		*poly;
	int			i;

	poly = map->polys;
	i = -1;
	while (poly && ++i)
	{
		poly->box_x = (t_dot){win->w, 0};
		poly->box_y = (t_dot){win->h, 0};
		poly->n_dot = N_DOTS_POLY;
		poly->n_proj = N_DOTS_POLY;

		create_dot_axe_x(poly->dots[0], poly->dots[1]);
		create_dot_axe_x(poly->dots[1], poly->dots[2]);
		create_dot_axe_x(poly->dots[2], poly->dots[3]);
		create_dot_axe_x(poly->dots[3], poly->dots[0]);

		set_proj(win, poly, poly->dots[0]);
		set_proj(win, poly, poly->dots[1]);
		set_proj(win, poly, poly->dots[2]);
		set_proj(win, poly, poly->dots[3]);
		set_proj(win, poly, poly->dots[4]);
		set_proj(win, poly, poly->dots[5]);

		poly_reduction(poly);

		set_box(win, &(poly->box_x), &(poly->box_y), poly->dots[], poly->dots_proj[]);
		set_box(win, &(poly->box_x), &(poly->box_y), poly->dots[], poly->dots_proj[]);
		set_box(win, &(poly->box_x), &(poly->box_y), poly->dots[], poly->dots_proj[]);
		set_box(win, &(poly->box_x), &(poly->box_y), poly->dots[], poly->dots_proj[]);

		printf("box xy %d %d / %d %d\n", poly->box_x.x, poly->box_x.y, poly->box_y.x, poly->box_y.y);
		poly = poly->next;
	}
}
// static void			set_box(t_win *win, t_dot *box_x, t_dot *box_y, t_fdot_3d dot, t_dot *proj)
// {
// 	proj->x = (win->map->player.fov_2 + dot.y / dot.x) * win->w / win->map->player.fov;
// 	proj->y = (win->map->player.fov_up_2 - dot.z / dot.x) * win->h / win->map->player.fov_up;
// 	if (proj->x < box_x->x)
// 		box_x->x = proj->x;
// 	if (proj->x > box_x->y)
// 		box_x->y = proj->x;
// 	if (proj->y < box_y->x)
// 		box_y->x = proj->y;
// 	if (proj->y > box_y->y)
// 		box_y->y = proj->y;
// }

// static void		set_proj(t_win *win, t_poly *poly, t_fdot_3d d1, t_fdot_3d d2, t_dot *proj_d1)
// {
// 	t_fdot_3d	tmp;

// 	if (d1.x > 0)
// 	{
// 		set_box(win, &(poly->box_x), &(poly->box_y), d1, proj_d1);
// 		if (d2.x < 0)
// 		{
// 			set_box(win, &(poly->box_x), &(poly->box_y), tmp = intersection_axe_x(d1, d2), &(poly->proj_tmp[poly->index_proj++]));
// 			printf("Dot %f %f %f\n", tmp.x, tmp.y, tmp.z);
// 		}
// 	}
// 	else
// 	{
// 		if (d2.x > 0)
// 		{
// 			set_box(win, &(poly->box_x), &(poly->box_y), tmp = intersection_axe_x(d1, d2), &(poly->proj_tmp[poly->index_proj++]));
// 			printf("Dot %f %f %f\n", tmp.x, tmp.y, tmp.z);
// 		}
// 		else
// 			return ;
// 	}
// }

// void			surround_walls(t_win *win, t_map *map)
// {
// 	t_poly		*poly;
// 	int			i;

// 	poly = map->polys;
// 	i = -1;
// 	while (poly && ++i)
// 	{
// 		poly->box_x = (t_dot){win->w, 0};
// 		poly->box_y = (t_dot){win->h, 0};
// 		poly->index_proj = 0;
// 		set_proj(win, poly, poly->dots[0], poly->dots[1], &(poly->dots_proj[0]));
// 		set_proj(win, poly, poly->dots[1], poly->dots[2], &(poly->dots_proj[1]));
// 		set_proj(win, poly, poly->dots[2], poly->dots[3], &(poly->dots_proj[2]));
// 		set_proj(win, poly, poly->dots[3], poly->dots[0], &(poly->dots_proj[3]));
// 		printf("box xy %d %d / %d %d\n", poly->box_x.x, poly->box_x.y, poly->box_y.x, poly->box_y.y);
// 		poly = poly->next;
// 	}
// }
