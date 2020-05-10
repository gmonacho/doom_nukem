/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 16:34:00 by gal               #+#    #+#             */
/*   Updated: 2020/05/10 16:34:00 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "ui.h"
#include "ui_error.h"
#include "libft.h"

static void		dead_change_loop(void *argument)
{
	t_arg_menu	*arg_menu;

	arg_menu = (t_arg_menu*)argument;
	*(arg_menu->loop) = arg_menu->value;
}

static void		set_menu_button_function(t_winui *winui, int *next_loop)
{
	ui_set_simple_button_function(winui,
									"b_menu",
									&dead_change_loop,
									&(t_arg_menu){next_loop, 1});
	ui_set_simple_button_function(winui,
									"b_exit",
									&dead_change_loop,
									&(t_arg_menu){next_loop, 2});
}

int				init_dead_menu(t_win *win)
{
	if (!(win->winui->ui.button_font = ui_load_font("TTF/DooM.ttf", 100)))
		return (0);
	if (!ui_load("interfaces/menu_dead", win->winui))
		return (0);
	SDL_SetRelativeMouseMode(SDL_FALSE);
	return (1);
}

static void		main_menu_quit(t_win *win, Uint32 ms)
{
	ui_free_ui(&win->winui->ui);
	SDL_Delay(ms);
}

int				dead_menu(t_win *win, t_player *player)
{
	int next_loop;
	int flag;

	flag = 0;
	next_loop = 0;
	if (init_dead_menu(win) == 0)
		return (ret_error("dead_menu"));
	if (player->currentHp <= 0)
	{
		while (!next_loop)
		{
			if (!flag)
			{
				set_menu_button_function(win->winui, &next_loop);
				flag = 1;
			}
			dead_menu_ui(win);
			if (next_loop != 0)
			{
				main_menu_quit(win, 250);
				return (next_loop);
			}
		}
	}
	return (0);
}
