#include "doom_nukem.h"
#include "ui_draw.h"

void	ed_display_player(t_win *win, const t_map *map)
{
	t_dot		pos;

	pos = ed_get_display_point(map, (t_dot){map->player.pos_up.x, map->player.pos_up.y});

	ui_set_draw_color(win->winui->rend, &(SDL_Color){225, 0, 225, 255});
	draw_circle(win, (t_circle){pos.x, pos.y, map->player.width * map->editor.unit / 2});
}