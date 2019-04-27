#include "loop.h"
#include "data.h"
#include "display.h"
#include "event.h"

static void			editor_display(t_win *win, const t_map *map)
{
	t_dot		p1;
	t_dot		p2;
	t_linedef	*line_tmp;

	SDL_SetRenderDrawColor(win->rend, 200, 200, 200, 255);
	p1 = (t_dot){map->x, map->y};
	p2 = (t_dot){map->x, map->y + map->h * map->unit};
	draw_line(win, p1, p2);
	p1 = (t_dot){map->x, map->y};
	p2 = (t_dot){map->x + map->w * map->unit, map->y};
	draw_line(win, p1, p2);
	p1 = (t_dot){map->x + map->w * map->unit, map->y};
	p2 = (t_dot){map->x + map->w * map->unit, map->y + map->h * map->unit};
	draw_line(win, p1, p2);
	p1 = (t_dot){map->x + map->w * map->unit, map->y + map->h * map->unit};
	p2 = (t_dot){map->x, map->y + map->h * map->unit};
	draw_line(win, p1, p2);
	SDL_SetRenderDrawColor(win->rend, 150, 150, 150, 200);
	draw_rect(win, map->rect_util);
	line_tmp = map->lines;
	while (line_tmp)
	{
		//printf("line_tmp->flags = %d\n", line_tmp->flags);
		if (line_tmp->flags & LINEDEF_SELECTED)
			SDL_SetRenderDrawColor(win->rend, 0, 175, 175, 255);
		else if (line_tmp->flags & LINEDEF_MOUSE_POINTED)
			SDL_SetRenderDrawColor(win->rend, 50, 150, 150, 255);
		else
			SDL_SetRenderDrawColor(win->rend, 200, 200, 200, 255);
		p1 = (t_dot){line_tmp->p1.x * map->unit + map->x, line_tmp->p1.y * map->unit + map->y};
		p2 = (t_dot){line_tmp->p2.x * map->unit + map->x, line_tmp->p2.y * map->unit + map->y};
		if (is_in_screen(win, p1) || is_in_screen(win, p2))
			draw_line(win, p1, p2);
		line_tmp = line_tmp->next;
	}
	draw_fps();
}

static int		editor_init(t_win *win, t_map *map)
{
	win->mouse = mouse_refresh();
	map->x = 0;
	map->y = 0;
	map->w = 2000;
	map->h = 1200;
	map->unit = 1.0;
	map->sectors = NULL;
	map->lines = NULL;
	map->rect_util = (SDL_Rect){};
	map->flags = 0;
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