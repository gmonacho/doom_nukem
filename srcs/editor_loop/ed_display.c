#include "doom_nukem.h"
#include "ui_draw.h"

static void			ed_display_polys_flat(t_win *win, const t_map *map)
{
	t_poly		*poly;
	SDL_Color	color;

	poly = map->polys;
	while (poly)
	{
		color = ed_get_flat_display_color(map, poly);
		ui_set_draw_color(win->rend, &color);
		if (ed_is_flat(poly) && ed_is_poly_printable(map, poly))
			ed_display_flat(win, map, poly);	
		poly = poly->next;
	}
}

static void			ed_display_polys_inclined(t_win *win, const t_map *map)
{
	t_poly		*poly;
	SDL_Color	color;

	color = (SDL_Color){255, 0, 0, 255};
	ui_set_draw_color(win->rend, &color);
	poly = map->polys;
	while (poly)
	{
		if (ed_is_inclined(poly) && ed_is_poly_printable(map, poly))
			ed_display_inclined(win, map, poly);
		poly = poly->next;
	}
}

static void			ed_display_polys_wall(t_win *win, const t_map *map)
{
	t_poly		*poly;
	SDL_Color	color;

	poly = map->polys;
	while (poly)
	{
		color = ed_get_wall_display_color(map, poly);
		ui_set_draw_color(win->rend, &color);
		if (ed_is_wall(poly) && ed_is_poly_printable(map, poly))
			ed_display_wall(win, map, poly);
		poly = poly->next;
	}
}

static void			ed_display_placing_poly(t_win *win, const t_map *map)
{
	ui_set_draw_color(win->winui->rend,
					&(SDL_Color){255, 225, 0, 255});
	if (map->editor.placing_poly)
	{
		if (ed_is_wall(map->editor.placing_poly))
			ed_display_wall(win, map, map->editor.placing_poly);
		else if (ed_is_flat(map->editor.placing_poly))
			ed_display_flat(win, map, map->editor.placing_poly);
		else if (ed_is_inclined(map->editor.placing_poly))	
			ed_display_inclined(win, map, map->editor.placing_poly);
		else
			printf("poly z : %f %f %f %f\n", map->editor.placing_poly->dots[0].z,
											map->editor.placing_poly->dots[1].z,
											map->editor.placing_poly->dots[2].z,
											map->editor.placing_poly->dots[3].z);
	}
}

static void			ed_display_polys(t_win *win, const t_map *map)
{
	ed_display_polys_flat(win, map);
	ed_display_polys_inclined(win, map);
	ed_display_polys_wall(win, map);
	ed_display_mobs(win, map);
	ed_display_selected_poly(win, map);
	ed_display_selected_mob(win, map);
	ed_display_placing_poly(win, map);
}

static void			ed_display_mouse_position(t_win *win, const t_map *map)
{
	t_text_area		*text_area;
	char			*tmp;

	if (map->editor.selected_poly)
	{
		text_area = ui_get_text_area_button(win->winui, "b_poly_dot_0");
		if (text_area)
		{
			ft_strdel(&text_area->text);
			tmp = ft_itoa(map->editor.selected_poly->dots[0].z);
			text_area->text = ft_strjoin("dot[0] = ", tmp);
		}
		text_area = ui_get_text_area_button(win->winui, "b_poly_dot_1");
		if (text_area)
		{
			ft_strdel(&text_area->text);
			tmp = ft_itoa(map->editor.selected_poly->dots[1].z);
			text_area->text = ft_strjoin("dot[1] = ", tmp);
		}
		text_area = ui_get_text_area_button(win->winui, "b_poly_dot_2");
		if (text_area)
		{
			ft_strdel(&text_area->text);
			tmp = ft_itoa(map->editor.selected_poly->dots[2].z);
			text_area->text = ft_strjoin("dot[2] = ", tmp);
		}
		text_area = ui_get_text_area_button(win->winui, "b_poly_dot_3");
		if (text_area)
		{
			ft_strdel(&text_area->text);
			tmp = ft_itoa(map->editor.selected_poly->dots[3].z);
			text_area->text = ft_strjoin("dot[3] = ", tmp);
		}
	}
}

static void			ed_display_selection_rect(t_win *win, const t_map *map)
{
	t_dot	pos;

	pos = ed_get_display_point(map, (t_dot){map->editor.select_rect.x,
											map->editor.select_rect.y});
	ui_set_draw_color(win->winui->rend, &(SDL_Color){225, 225, 225, 255});
	if (win->winui->mouse.clicked & UI_MOUSE_RIGHT ||
		(map->editor.flags & ED_SELECTION
		&& win->winui->mouse.clicked & UI_MOUSE_LEFT))
		ui_draw_rect(win->winui->rend, &(t_rect){
		pos.x,
		pos.y,
		map->editor.select_rect.w * map->editor.unit,
		map->editor.select_rect.h * map->editor.unit});
}

void			ed_display(t_win *win, const t_map *map)
{
	ed_display_polys(win, map);
	ed_display_objects(win, map);
	ed_display_player(win, map);
	ed_display_selected_obj(win, map);
	ed_display_mouse_position(win, map);
	ed_display_selection_rect(win, map);
	draw_fps();
}