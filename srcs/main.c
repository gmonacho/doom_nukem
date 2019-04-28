#include "doom_nukem.h"

static int	init_equations(t_map *map)
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
}

static int	init(t_map *map, t_player *player)
{
	init_equations(map);
	player->hitbox = 0.5;
	player->pos = (t_fdot){0, 0};
	player->dir = PI / 2;
	player->const_vel = 0.02;
	return (SUCCESS);
}

int		main(int ac, char **av)
{
	t_win	win;
	t_map	map;

	if (init(&map, &(map.player)) < 0)
		return (ret_error("Error init"));
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (ret_error(SDL_GetError()));

	if (!(create_window(&win, "doom_nukem", (SDL_Rect){200, 200, 2000, 1200}, SDL_WINDOW_SHOWN)))
		return (0);
	SDL_SetRenderDrawColor(win.rend, 255, 255, 255, 255);

	if (ac >= 2 && !ft_atoi(av[1]))
		editor_loop(&win);
	else
		game_loop(&win);

	SDL_DestroyWindow(win.ptr);
	SDL_DestroyRenderer(win.rend);
	SDL_Quit();
	return (SUCCESS);
}