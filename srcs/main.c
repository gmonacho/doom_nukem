#include <fcntl.h>
#include "doom_nukem.h"

/*static int	init_equations(t_map *map)
{
	t_sector	*sector;
	t_linedef	*line;
	sector = map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			line->equation.a = (line->p2.y - line->p1.y) /\
								(line->p2.x - line->p1.x);
			line->equation.b = line->p1.y - line->equation.a * line->p1.x;
			line = line->next;
		}
		sector = sector->next;
	}
	return (SUCCESS);
}*/

static int	init(t_win *win, t_map *map, t_player *player)
{
	//t_linedef	*tmp;

	//init_equations(map);
	map = NULL;
	win->w = 2000;
	win->h = 1000;
	player->pos = (t_fdot){win->w / 2, win->h / 2};
	player->const_vel = 1;
	player->dir = M_PI_2;
	player->hitbox = 10;
	player->sector = 0;
	/*add_sector(&map->sectors, new_sector());

	tmp = new_linedef((t_line){(t_dot){win->w / 6, win->h / 4},\
								(t_dot){win->w / 6, 3 * win->h / 4}},\
						"Michel",\
						NULL, WALL);
	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){win->w / 6, 3 * win->h / 4},\
								(t_dot){2 * win->w / 6, 3 * win->h / 4}},\
						"Jacquie",\
						NULL, WALL);
	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){2 * win->w / 6, 3 * win->h / 4},\
								(t_dot){2 * win->w / 6, win->h / 4}},\
						"Ma bite",\
						NULL, PORTAL);
	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){2 * win->w / 6, win->h / 4},\
								(t_dot){win->w / 6, win->h / 4}},\
						"Seb",\
						NULL, WALL);

	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){4 * win->w / 6, win->h / 4},\
								(t_dot){4 * win->w / 6, 3 * win->h / 4}},\
						"ag",\
						NULL, PORTAL);
	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){4 * win->w / 6, 3 * win->h / 4},\
								(t_dot){5 * win->w / 6, 3 * win->h / 4}},\
						"agio",\
						NULL, WALL);
	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){5 * win->w / 6, 3 * win->h / 4},\
								(t_dot){5 * win->w / 6, win->h / 4}},\
						"agiord",\
						NULL, WALL);
	add_linedef(&map->sectors->lines, tmp);
	tmp = new_linedef((t_line){(t_dot){5 * win->w / 6, win->h / 4},\
								(t_dot){4 * win->w / 6, win->h / 4}},\
						"agiordan",\
						NULL, WALL);
	add_linedef(&map->sectors->lines, tmp);*/

	return (1);
}

int			main(int argc, char **argv)
{
	int		fd;
	int		fd1;
	t_win	win;
	t_map	map;

	if (!argc)
		argc = 0;
	if ((((fd = open(argv[1] , O_RDONLY)) <= 0) || ((fd1 = open(argv[1], O_RDONLY)) <= 0)))
		return (ret_error("open error"));
	map.sectors = ft_data_storing(fd, fd1);
	if (init(&win, &map, &(map.player)) < 0)
		return (ret_error("init error"));
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() == -1)
		return (ret_error(SDL_GetError()));
	if (!(create_window(&win, "doom_nukem", (SDL_Rect){200, 200, 2000, 1000}, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)))
		return (0);
	SDL_SetRenderDrawColor(win.rend, 255, 255, 255, 255);
	editor_loop(&win);
	// game_loop(&win, &map);
	SDL_DestroyWindow(win.ptr);
	SDL_DestroyRenderer(win.rend);
	SDL_Quit();
	return (0);
}