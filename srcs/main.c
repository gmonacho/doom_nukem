/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 00:42:47 by gal               #+#    #+#             */
/*   Updated: 2020/05/17 22:18:14 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "ui_error.h"

/*
**	A faire :
**
**	Elements de decors
**	Tenter de casser le parsing
**	Clean code/fichier inutiles
**	Gerer les leaks
**	Norme all
**
*/

//Enlever les wildcards

static void			init_map(t_map *map)
{
	map->objects = NULL;
	map->polys = NULL;
	map->polys_save = NULL;
	map->mob = NULL;
	map->player.width = 30;
	map->player.height = 30;
	map->player.inventory = NULL;
	map->player.rays = NULL;
	map->editor.settings.texture = NULL;
}

// static void		set_hud_to_null(t_win *win)
// {
// 	int			i;

// 	i = 0;
// 	while (i < 16)
// 		win->texhud->tex[i++] = NULL;
// 	i = 0;
// 	while (i < 6)
// 		win->texhud->tex_weapon[i++] = NULL;
// 	i = 0;
// 	while (i < 5)
// 		win->texhud->tex_reload[i++] = NULL;
// 	win->texhud->police = NULL;
// }

static int			pre_init(t_win *win, t_map *map)
{
	SDL_DisplayMode	screen;

	win->w = WIDTH;
	win->h = HEIGHT;
	win->map = map;
	map->save.if_pars = 0;
	init_map(map);
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1 ||\
		TTF_Init() == -1 ||\
		(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG ||\
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT,\
				MIX_DEFAULT_CHANNELS, 1024) == -1 ||\
		(Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3) != MIX_INIT_MP3 ||\
		SDL_GetDesktopDisplayMode(0, &screen) < 0)
		return (ui_ret_error("Pre_init failed", SDL_GetError(), 1));
	if (!create_window(win, "doom_nukem",\
						(SDL_Rect){screen.w / 2 - win->w / 2,\
									screen.h / 2 - win->h / 2, win->w, win->h},\
						SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))
		return (1);
	if (!init_music_timer(&(win->music)))
		return (ui_ret_error("init_music_timer failed", NULL, 1));
	return (0);
}

static int			init(t_win *win, t_map *map, t_player *player)
{
	if (init_win_player(win, player))
		return (1);
	init_polygone(map->polys);
	if (check_parallelogram(map->polys))
		return (ui_ret_error("init", "A poly is not a parallelogramme", 1));
	create_poly_save(map);
	if (init_threads(win, map, player))
		return (1);
	return (0);
}

int					main(int argc, char **argv)
{
	int				ret;
	t_win			win;
	t_map			map;

	if (pre_init(&win, &map))
		ft_putendl("Pre init failed, exit.");
	else if (argc == 1)
		editor_loop(&win, &map);
	else if (argc == 2)
	{
		if ((ret = open(argv[1], O_RDONLY)) >= 0 &&\
			(win.map->polys = ft_data_storing(ret, &map, &win)))
		{
			if ((ret = init(&win, &map, &(map.player))))
				return (ret_num_error("Init error", ret));
			main_menu(&win, &map, argv[1]);
		}
		else
			ui_ret_error("main", "open or ft_data_storing failed", 1);
	}
	else
		ft_putendl("usage: ./doom_nukem [map]");
	main_free(&win, &map);
	return (0);
}
