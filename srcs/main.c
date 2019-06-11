#include "doom_nukem.h"

static int		find_portal_id(t_map *map, t_linedef *line1, int id)
{
	t_sector	*sector;
	t_linedef	*line;

	sector = map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			if (id == line->id && line1 != line)
			{
				line1->destline = line;
				return (0);
			}
			line = line->next;
		}
		sector = sector->next;
	}
	return (1);
}

static int		init_lines(t_map *map)
{
	t_sector	*sector;
	t_linedef	*line;

	sector = map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			line->sector = sector;
			if (line->flags & PORTAL && find_portal_id(map, line, line->id))
				return (1);
			line = line->next;
		}
		sector = sector->next;
	}
	return (0);
}

static int		init_sectors(t_map *map, t_player *player)
{
	t_sector	*sector;
	int			i;

	sector = map->sectors;
	while (sector)
	{
		if (sector->floor_height >= sector->ceil_height)
			return (1);
		sector->height = sector->ceil_height - sector->floor_height;
		sector = sector->next;
	}
	//player->numsector = 1;	//A parser
	player->sector = map->sectors; //A parser
	i = -1;
	while (++i < player->numsector)
		player->sector = player->sector->next;
	/*if (player->height > player->sector->height)
		return (1);*/
	return (0);
}

static int		init(t_win *win, t_map *map, t_player *player)
{
	if (init_sectors(map, player))
		return (1);
	if (init_lines(map))
		return (2);
	win->w = 1200;
	win->h = 1000;
	//player->pos = (t_fdot){2 * win->w / 3, win->h / 2};
	//player->const_vel = 10;
	player->dir = M_PI;
	player->orientation = win->h / 2;
	player->fov = M_PI / 2;//3 * M_PI / 4;
	//player->width = 20;
	//player->height = 100;

	/*add_sector(&map->sectors, new_sector());

	tmp = new_linedef((t_line){(t_dot){win->w / 6, win->h / 4},\
								(t_dot){win->w / 6, 3 * win->h / 4}},\
						NULL, WALL);
	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){win->w / 6, 3 * win->h / 4},\
								(t_dot){2 * win->w / 6, 3 * win->h / 4}},\
						NULL, WALL);
	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){2 * win->w / 6, 3 * win->h / 4},\
								(t_dot){2 * win->w / 6, win->h / 4}},\
						NULL, PORTAL);
	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){2 * win->w / 6, win->h / 4},\
								(t_dot){win->w / 6, win->h / 4}},\
						NULL, WALL);

	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){4 * win->w / 6, win->h / 4},\
								(t_dot){4 * win->w / 6, 3 * win->h / 4}},\
						NULL, PORTAL);
	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){4 * win->w / 6, 3 * win->h / 4},\
								(t_dot){5 * win->w / 6, 3 * win->h / 4}},\
						NULL, WALL);
	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){5 * win->w / 6, 3 * win->h / 4},\
								(t_dot){5 * win->w / 6, win->h / 4}},\
						NULL, WALL);
	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){5 * win->w / 6, win->h / 4},\
								(t_dot){4 * win->w / 6, win->h / 4}},\
						NULL, WALL);
	add_linedef(&map->sectors->lines, tmp);*/

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
	if ((((fd = open(argv[1], O_RDONLY)) <= 0) ||\
	((fd1 = open(argv[1], O_RDONLY)) <= 0)))
		return (ret_error("open error"));

	map.sectors = ft_data_storing(fd, fd1, &map, &(map.player));

	if ((ret = init(&win, &map, &(map.player))))
		return (ret_num_error("Init error", ret));


	if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() == -1)
		return (ret_error(SDL_GetError()));

	if (!(create_window(&win, "doom_nukem", (SDL_Rect){200, 200, win.w, win.h}, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)))
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