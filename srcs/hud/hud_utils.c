/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 16:34:07 by gal               #+#    #+#             */
/*   Updated: 2020/05/17 23:42:37 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "ui.h"
#include "ui_error.h"
#include "libft.h"

void		dead_menu_ui(t_win *win)
{
	ui_set_draw_color(win->rend, &(SDL_Color){71, 27, 27, 255});
	ui_clear_win(win->winui);
	ui_display_frames(win->winui, win->winui->ui.frames);
	ui_draw_rend(win->winui);
	ui_poll_event(&win->winui->event);
	ui_update_ui(win->winui);
}

void		define_line_shot(t_win *win, t_player *player)
{
	player->bullet_drop = NULL;
	player->len_bullet = 0;
	player->l[1] = is_point(win->w * 0.68, win->h * 0.65,
		win->w * 0.637, win->h * 0.625);
	player->l[2] = is_point(win->w * 0.637, win->h * 0.625,
		win->w * 0.595, win->h * 0.6);
	player->l[3] = is_point(win->w * 0.595, win->h * 0.6,
		win->w * 0.552, win->h * 0.575);
	player->l[4] = is_point(win->w * 0.552, win->h * 0.575,
		win->w * 0.51, win->h * 0.55);
}
