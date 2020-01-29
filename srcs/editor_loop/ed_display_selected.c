#include "doom_nukem.h"
#include "ui_draw.h"

// static void	ed_draw_inclined_height()
// static void	ed_draw_flat_height()

// static void	ed_draw_wall_height(t_win *win, const t_map *map, t_poly *poly)
// {	
// }

void		ed_display_selected_poly(t_win *win, const t_map *map)
{
	if (map->editor.selected_poly
		&& ed_is_poly_printable(map, map->editor.selected_poly))
	{
		ui_set_draw_color(win->winui->rend, &(SDL_Color){0, 200, 200, 255});
		if (ed_is_wall(map->editor.selected_poly))
			ed_display_wall(win, map, map->editor.selected_poly);
		else if (ed_is_inclined(map->editor.selected_poly))
			ed_display_inclined(win, map, map->editor.selected_poly);
		else if (ed_is_flat(map->editor.selected_poly))
			ed_display_flat(win, map, map->editor.selected_poly);
	}
}
