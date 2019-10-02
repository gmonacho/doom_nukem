#include "doom_nukem.h"

static int		init(t_win *win, t_map *map, t_player *player)
{
	if (init_textures(&(map->textures)))
	{
		ft_putendl("init_textures failed");
		return (1);
	}
	if (init_sectors(map, player))
	{
		ft_putendl("init_sectors failed");
		return (2);
	}
	if (init_lines(map))
	{
		ft_putendl("init_lines failed");
		return (3);
	}
	init_player(win, player);
	return (0);
}

int			main(int argc, char **argv)
{
	int		fd;
	int		fd1;
	int		next_loop;
	int		ret;
	t_win	win;
	t_map	map;

	if (argc == 1 || argc == 2)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() == -1 || (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
			return (ret_error(SDL_GetError()));
		win.w = 1000;
		win.h = 800;
		if (!(create_window(&win, "doom_nukem", (SDL_Rect){200, 100, win.w, win.h}, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)))
			return (0);
		if (argc == 1)
			editor_loop(&win, NULL);
		else
		{
			if ((((fd = open(argv[1], O_RDONLY)) <= 0) ||
			((fd1 = open(argv[1], O_RDONLY)) <= 0)))
				return (ret_error("open error"));
			map.sectors = ft_data_storing(fd, fd1, &map, &(map.player));
			if ((ret = init(&win, &map, &(map.player))))
				return (ret_num_error("Init error", ret));
			next_loop = main_menu(&win);
			if (next_loop == 2)
				game_loop(&win, &map);
			else if (next_loop == 3)
				editor_loop(&win, &map);
		}
		SDL_DestroyWindow(win.ptr);
		SDL_DestroyRenderer(win.rend);
		SDL_Quit();
	}
	return (0);
}
