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
			line->equation.a = (line->d2.y - line->d1.y) /\
								(line->d2.x - line->d1.x);
			line->equation.b = line->d1.y - line->equation.a * line->d1.x;
			line = line->next;
		}
		sector = sector->next;
	}
	return (SUCCESS);
}*/

static int	init(t_win *win, t_map *map, t_player *player)
{
	t_linedef	*tmp;

	//init_equations(map);
	win->width = 2000;
	win->height = 1000;
	player->hitbox = 10;
	player->pos = (t_fdot){win->width / 2, win->height / 2 - 100};
	player->sector = 0;
	player->dir = PI / 2;
	player->const_vel = 0.8;
	if (!(map->sectors = (t_sector *)malloc(sizeof(t_sector))))
		return (MALLOC_ERROR);
	map->sectors->lines = NULL;
	map->sectors->next = NULL;
	tmp = new_linedef((t_line){(t_fdot){0, 0},\
										(t_fdot){win->width, win->height}},\
										NULL, 0);
	map_add_line(map, 0, tmp);
	tmp = new_linedef((t_line){(t_fdot){0, win->height},\
										(t_fdot){win->width, 0}},\
										NULL, 0);
	map_add_line(map, 0, tmp);
	return (SUCCESS);
}

int		main(int ac, char **av)
{
	t_win	win;
	t_map	map;

	if (init(&win, &map, &(map.player)) < 0)
		return (ret_error("Error init"));
	printf("Fin init main\n");
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (ret_error(SDL_GetError()));
	printf("Fin SDL init main\n");

	if (!(create_window(&win, "doom_nukem", (SDL_Rect){0, 0, win.width, win.height}, SDL_WINDOW_SHOWN)))
		return (0);
	printf("Fin create window main\n");

	if (ac >= 2 && !ft_atoi(av[1]))
		;//editor_loop(&win);
	else
		game_loop(&win, &map);
	printf("Fin game loop main\n");

	SDL_DestroyWindow(win.ptr);
	SDL_DestroyRenderer(win.rend);
	SDL_Quit();
	return (SUCCESS);
}