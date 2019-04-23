#include "loop.h"
#include "data.h"
#include "display.h"
#include "event.h"

static void			editor_display(t_win *win, const t_map *map)
{
	int		i;
	t_dot	p1;
	t_dot	p2;

	i = 0;
	SDL_SetRenderDrawColor(win->rend, 200, 200, 200, 200);
	while (i < map->nb_lines)
	{
		p1 = (t_dot){(map->lines[i].p1.x) * map->unit - map->x, map->lines[i].p1.y * map->unit - map->y};
		p2 = (t_dot){map->lines[i].p2.x * map->unit - map->x, map->lines[i].p2.y * map->unit - map->y};
		draw_line(win, p1, p2);
		i++;
	}
	draw_fps();
}

static int		editor_init(t_win *win, t_map *map)
{
	win->mouse = mouse_refresh();
	map->x = 0;
	map->y = 0;
	map->w = 30;
	map->h = 30;
	map->unit = 100;
	map->sectors = NULL;
	map->nb_vectors = 0;
	map->lines = NULL;
	map->nb_lines = 0;
	return (1);
}

int				editor_loop(t_win *win)
{
	SDL_bool	loop;
	t_map		map;

	editor_init(win, &map);
	loop = SDL_TRUE;
	while (loop)
	{			
		clear_rend(win->rend, 30, 30, 30);
		editor_display(win, &map);
		editor_event(win, &map, &loop);
		SDL_RenderPresent(win->rend);
	}
	return (1);
}