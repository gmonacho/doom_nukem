#include "doom_nukem.h"

static int		init(t_win *win, t_map *map, t_player *player)
{
	if (init_sectors(map, player))
		return (1);
	if (init_lines(map))
		return (2);
	win->w = 1000;
	win->h = 800;
	player->dir = M_PI / 2;
	player->orientation = 1 * win->h / 2;
	player->fov = M_PI / 2;
	player->maxHp = 50;
	player->currentHp = player->maxHp;
	player->maxArmor = 50;
	player->currentArmor = player->maxArmor;
	player->ammo = 30;
	player->magazine = 60;
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

	if (!argc)
		argc = 0;
	if ((((fd = open(argv[1], O_RDONLY)) <= 0) ||
	((fd1 = open(argv[1], O_RDONLY)) <= 0)))
		return (ret_error("open error"));

	map.sectors = ft_data_storing(fd, fd1, &map, &(map.player));

	if ((ret = init(&win, &map, &(map.player))))
		return (ret_num_error("Init error", ret));
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() == -1)
		return (ret_error(SDL_GetError()));

	if (!(create_window(&win, "doom_nukem", (SDL_Rect){200, 100, win.w, win.h}, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)))
		return (0);
	//SDL_SetRenderDrawColor(win.rend, 255, 255, 255, 255);

	next_loop = main_menu(&win);
	if (next_loop == 2)
		game_loop(&win, &map);
	else if (next_loop == 3)
		editor_loop(&win);

	SDL_DestroyWindow(win.ptr);
	SDL_DestroyRenderer(win.rend);
	SDL_Quit();
	return (0);
}