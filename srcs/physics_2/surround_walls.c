#include "doom_nukem.h"

/*
**	tan(a) = dx / dy;
**	On oublie le tan pour annuler le fish-eyes
*/

// static void		update_poly_2d(t_dot *poly_2d_origin, int *poly_w, int *poly_h, t_dot dot)
// {
// }

static void		_3d_to_2d(t_win *win, t_player *player, t_linedef *line, t_fdot_3d dot_3d)
{
	t_dot		proj_2d;
	// double		alpha;
	// double		alpha_up;
	// double		dx;
	// double		dy;

	// if (is_null(dx = dot_3d.x, 0.005))
	// 	alpha = 0;
	// else if (dot_3d.x < 0)
	// 	alpha = dot_3d.y / dot_3d.x;
	// else
		// alpha = dot_3d.y / dot_3d.x;
	proj_2d.x = (player->fov_2 + dot_3d.y / dot_3d.x) * win->w / player->fov;
	proj_2d.y = (player->fov_up_2 - dot_3d.z / dot_3d.x) * win->h / player->fov_up;

	if (proj_2d.x < line->poly_2d_origin->x)
		line->poly_2d_origin->x = proj_2d.x;
	if (proj_2d.y < line->poly_2d_origin->y)
		line->poly_2d_origin->y = proj_2d.y;

	if (proj_2d.x - line->poly_2d_origin->x > line->poly_w)
		line->poly_w = proj_2d.x - line->poly_2d_origin->x;
	if (proj_2d.y - line->poly_2d_origin->y > line->poly_h)
		line->poly_h = proj_2d.y - line->poly_2d_origin->y;
}

void			surround_walls(t_win *win, t_map *map, t_player *player, t_sector *sector)
{
	t_linedef	*line;

	line = sector->lines;
	while (line)
	{
		line->poly_2d_origin = (t_dot){win->w, win->h};
		line->poly_w = 0;
		line->poly_h = 0;
		_3d_to_2d(win, player, line, line->poly_3d[0]);
		_3d_to_2d(win, player, line, line->poly_3d[1]);
		_3d_to_2d(win, player, line, line->poly_3d[2]);
		_3d_to_2d(win, player, line, line->poly_3d[3]);
		line = line->next;
	}
}
