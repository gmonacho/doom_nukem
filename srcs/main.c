#include "doom_nukem.h"

static int		init(t_win *win, t_map *map, t_player *player)
{
	printf("Debut init\n");
	if (init_textures(&(map->textures)))
	{
		ft_putendl("init_textures failed");
		return (1);
	}
	if (!init_music(&win->music))
	{
		ft_putendl("init_music failed");
		return (4);
	}
	init_polygone(map->polys);
	init_player(win, player);
	return (0);
}

int			main(int argc, char **argv)
{
	int			fd;
	int			fd1;
	int			next_loop;
	int			ret;
	t_win		win;
	t_map		map;
	SDL_bool	loop;
	SDL_Event	event;

	if (argc == 1 || argc == 2)
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1 || TTF_Init() == -1)
			return (ret_error(SDL_GetError()));
		if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
			return (ret_error(SDL_GetError()));
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
			return (ret_error(SDL_GetError()));
		if ((Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3) != MIX_INIT_MP3)
			return (ret_error(SDL_GetError()));
		win.w = WIDTH;
		win.h = HEIGHT;
		win.map = &map;
		if (!(create_window(&win, "doom_nukem", (SDL_Rect){200, 100, win.w, win.h}, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)))
			return (0);
		loop = SDL_TRUE;
		if (argc == 1)
		{
			init_music(&win.music);
			editor_loop(&win, NULL);
		}
		else
		{
			if (!(map.polys = polys_a_la_mano()))
				return (1);
			if ((((fd = open(argv[1], O_RDONLY)) <= 0) || ((
				fd1 = open(argv[1], O_RDONLY)) <= 0)))
				return (ret_error("open error"));
			SDL_PollEvent(&event);
			if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				loop = SDL_FALSE;
			// map.polys = ft_data_storing(fd, fd1, &map, &(map.player));
			if ((ret = init(&win, &map, &(map.player))))
				return (ret_num_error("Init error", ret));
			next_loop = main_menu(&win);
			if (next_loop == 2)
				game_loop(&win, &map);
			else if (next_loop == 3)
				editor_loop(&win, &map);
			else if (next_loop == 4) 
				loop = SDL_FALSE;
			else if (next_loop == 5)
				print_credit(&win);
		}
		SDL_DestroyWindow(win.ptr);
		SDL_DestroyRenderer(win.rend);
		SDL_Quit();
	}
	return (0);
}
