#include "doom_nukem.h"

/*
**	tan(a) = dx / dy;
**	On oublie le tan pour annuler le fish-eyes
*/

// static void		update_poly_2d(t_dot *poly_2d_origin, int *poly_2d_w, int *poly_2d_h, t_dot dot)
// {
// }

void			draw_projection(t_win *win)
{
	t_poly		*poly;

	poly = win->map->polys;
	while (poly)
	{
		SDL_SetRenderDrawColor(win->rend, 0xDD, 0x20, 0x20, 0xFF);
		draw_line(win, poly->proj_d1, poly->proj_d2);
		draw_line(win, poly->proj_d2, poly->proj_d3);
		draw_line(win, poly->proj_d3, poly->proj_d4);
		draw_line(win, poly->proj_d4, poly->proj_d1);
		poly = poly->next;
	}
}

void			draw_all_square(t_win *win)
{
	t_poly		*poly;

	poly = win->map->polys;
	while (poly)
	{
		SDL_SetRenderDrawColor(win->rend, 0xDD, 0x20, 0x20, 0x20);
		draw_line(win, poly->poly_2d_origin, (t_dot){poly->poly_2d_origin.x + poly->poly_2d_w, poly->poly_2d_origin.y});
		draw_line(win, poly->poly_2d_origin, (t_dot){poly->poly_2d_origin.x, poly->poly_2d_origin.y + poly->poly_2d_h});
		draw_line(win, (t_dot){poly->poly_2d_origin.x + poly->poly_2d_w, poly->poly_2d_origin.y}, (t_dot){poly->poly_2d_origin.x + poly->poly_2d_w, poly->poly_2d_origin.y + poly->poly_2d_h});
		draw_line(win, (t_dot){poly->poly_2d_origin.x, poly->poly_2d_origin.y + poly->poly_2d_h}, (t_dot){poly->poly_2d_origin.x + poly->poly_2d_w, poly->poly_2d_origin.y + poly->poly_2d_h});
		poly = poly->next;
	}
}

static void		_3d_to_2d(t_win *win, t_poly *poly, t_fdot_3d dot_3d, t_dot *proj)
{
	proj->x = (win->map->player.fov_2 + dot_3d.y / dot_3d.x) * win->w / win->map->player.fov;
	proj->y = (win->map->player.fov_up_2 - dot_3d.z / dot_3d.x) * win->h / win->map->player.fov_up;

	if (proj->x < poly->poly_2d_origin.x)
		poly->poly_2d_origin.x = proj->x;
	if (proj->y < poly->poly_2d_origin.y)
		poly->poly_2d_origin.y = proj->y;

	if (proj->x - poly->poly_2d_origin.x > poly->poly_2d_w)
		poly->poly_2d_w = proj->x - poly->poly_2d_origin.x;
	if (proj->y - poly->poly_2d_origin.y > poly->poly_2d_h)
		poly->poly_2d_h = proj->y - poly->poly_2d_origin.y;
}

void			surround_walls(t_win *win, t_map *map)
{
	t_poly		*poly;

	poly = map->polys;
	while (poly)
	{
		poly->poly_2d_origin = (t_dot){win->w, win->h};
		poly->poly_2d_w = 0;
		poly->poly_2d_h = 0;
		_3d_to_2d(win, poly, poly->d1, &(poly->proj_d1));
		_3d_to_2d(win, poly, poly->d2, &(poly->proj_d2));
		_3d_to_2d(win, poly, poly->d3, &(poly->proj_d3));
		_3d_to_2d(win, poly, poly->d4, &(poly->proj_d4));
		poly = poly->next;
	}
}
