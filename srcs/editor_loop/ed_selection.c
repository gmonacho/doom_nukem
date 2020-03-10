#include "doom_nukem.h"

static int	ed_first_fill_selected(t_win *win, t_map *map)
{
	if ((map->editor.selected_mob = ed_get_selected_mob(win, map)))
	{
		ed_set_buttons_mob(win, map->editor.selected_mob);
		map->editor.selected_obj = NULL;
		map->editor.selected_poly = NULL;
		map->editor.selected_player = NULL;
		return (1);
	}
	else if ((map->editor.selected_player = ed_get_selected_player(win, map)))
	{
		ed_set_buttons_player(win, map->editor.selected_player);
		map->editor.selected_obj = NULL;
		map->editor.selected_poly = NULL;
		map->editor.selected_mob = NULL;
		return (1);
	}
	return (0);
}

static int	ed_second_fill_selected(t_win *win, t_map *map)
{
	if ((map->editor.selected_obj = ed_get_selected_obj(win, map)))
	{
		ed_set_buttons_object(win, map->editor.selected_obj);
		map->editor.selected_mob = NULL;
		map->editor.selected_poly = NULL;
		map->editor.selected_player = NULL;
		return (1);
	}
	else if ((map->editor.selected_poly = ed_get_selected_poly(map, 0)))
	{
		if (ed_is_wall(map->editor.selected_poly))
			ed_set_buttons_wall(win, map->editor.selected_poly);
		else if (ed_is_flat(map->editor.selected_poly))
			ed_set_buttons_flat(win, map->editor.selected_poly);
		else if (ed_is_inclined(map->editor.selected_poly))
			ed_set_buttons_inclined(win, map->editor.selected_poly);
		map->editor.selected_mob = NULL;
		map->editor.selected_obj = NULL;
		map->editor.selected_player = NULL;
		return (1);
	}
	return (0);
}

static void	ed_third_fill_selected(t_win *win, t_map *map)
{
	ed_clean_property(win, 1);
	map->editor.selected_mob = NULL;
	map->editor.selected_poly = NULL;
	map->editor.selected_obj = NULL;
	map->editor.selected_player = NULL;
}

void		ed_selection(t_win *win, t_map *map)
{
	if (win->winui->mouse.clicking & UI_MOUSE_LEFT
		|| win->winui->mouse.clicking & UI_MOUSE_RIGHT)
	{
		map->editor.select_rect.x = ed_get_map_x(map, win->winui->mouse.pos.x);
		map->editor.select_rect.y = ed_get_map_y(map, win->winui->mouse.pos.y);
		map->editor.select_rect.w = 0;
		map->editor.select_rect.h = 0;
	}
	else if (win->winui->mouse.clicked & UI_MOUSE_LEFT
			|| win->winui->mouse.clicked & UI_MOUSE_RIGHT)
	{
		map->editor.select_rect.w = ed_get_map_x(map, win->winui->mouse.pos.x)
									- map->editor.select_rect.x;
		map->editor.select_rect.h = ed_get_map_y(map, win->winui->mouse.pos.y)
									- map->editor.select_rect.y;
		if (!ed_first_fill_selected(win, map))
		{
			if (!ed_second_fill_selected(win, map))
				ed_third_fill_selected(win, map);
		}
	}
}
