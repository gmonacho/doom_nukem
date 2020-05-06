/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ed_display.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 14:20:38 by gal               #+#    #+#             */
/*   Updated: 2020/05/06 14:20:38 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "ui_draw.h"

static void	ed_display_selection_rect(t_win *win, const t_map *map)
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

void		ed_display(t_win *win, const t_map *map)
{
	ed_display_polys(win, map);
	ed_display_objects(win, map);
	ed_display_player(win, map);
	ed_display_selected_obj(win, map);
	ed_display_selection_rect(win, map);
	draw_fps();
}
