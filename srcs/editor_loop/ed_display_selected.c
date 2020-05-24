/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ed_display_selected.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 14:20:35 by gal               #+#    #+#             */
/*   Updated: 2020/05/24 13:54:39 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "ui_draw.h"

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

void		ed_display_selected_mob(t_win *win, const t_map *map)
{
	t_dot	pos;

	if (map->editor.selected_mob)
	{
		ui_set_draw_color(win->winui->rend, &(SDL_Color){0, 200, 200, 255});
		pos = ed_get_display_point(map,
									(t_dot){map->editor.selected_mob->pos.x,
											map->editor.selected_mob->pos.y});
		draw_circle(win,
			(t_circle){pos.x, pos.y,
			map->editor.selected_mob->width * map->editor.unit / 2});
		ui_set_draw_color(win->winui->rend, &(SDL_Color){200, 255, 50, 255});
		draw_circle(win,
			(t_circle){pos.x, pos.y,
			map->editor.selected_mob->agro_dist * map->editor.unit});
	}
}

void		ed_display_selected_obj(t_win *win, const t_map *map)
{
	t_dot	pos;

	if (map->editor.selected_obj)
	{
		ui_set_draw_color(win->winui->rend, &(SDL_Color){0, 200, 200, 255});
		pos = ed_get_display_point(map,
						(t_dot){map->editor.selected_obj->pos_rotz_only.x,
						map->editor.selected_obj->pos_rotz_only.y});
		if (map->editor.selected_obj->type == DOOR)
			draw_quarter_circle(win, (t_circle){pos.x, pos.y,
							map->editor.selected_obj->width * map->editor.unit},
							map->editor.selected_obj->dir);
		else
			draw_circle(win, (t_circle){pos.x, pos.y,
				map->editor.selected_obj->width * map->editor.unit / 2});
	}
}
