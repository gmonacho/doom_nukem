#include "loop.h"
#include "data.h"
#include "display.h"
#include "event.h"

static void			editor_display(t_win *win, const t_map *map)
{
	t_dot		p1;
	t_dot		p2;
	t_linedef	*line_tmp;
	t_frame		*tmp_frame;
	t_button	*tmp_button;

	SDL_SetRenderDrawColor(win->rend, 100, 100, 100, 255);
	draw_rect(win, (SDL_Rect){map->x, map->y, map->w * map->unit, map->h * map->unit});
	SDL_SetRenderDrawColor(win->rend, 150, 150, 150, 200);
	if (map->flags & MAP_SELECTING)
		draw_rect(win, map->rect_util);
	line_tmp = map->lines;
	while (line_tmp)
	{
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
	SDL_SetRenderDrawColor(win->rend, 200, 200, 200, 255);
	tmp_frame = win->frames;
	while (tmp_frame)
	{
		if (tmp_frame->texture)
			printf("texture\n");
		else
			draw_ratio_rect(win, &(SDL_Rect){0, 0, win->w, win->h}, &tmp_frame->ratio);
		tmp_button = tmp_frame->buttons;
		while (tmp_button)
		{
			if (tmp_button->texture)
				printf("texture btn\n");
			else
				draw_ratio_rect(win, &tmp_frame->rect, &tmp_button->ratio);
			tmp_button = tmp_button->next;
		}
		tmp_frame = tmp_frame->next;
	}
	draw_fps();
}

static int		ui_init(t_win *win)
{
	// sector frame
	add_frame_to_window(win, new_frame((t_frect){0.05, 0.02, 0.9, 0.05}, NULL, FRAME_SECTORS, NULL));
	add_button_to_frame(&win->frames, new_button((t_frect){0, 0, 1.0 / MAX_SECTORS, 1}, NULL, 0));
	add_frame_to_window(win, new_frame((t_frect){0.02, 0.1, 0.15, 0.4}, NULL, FRAME_INFO, NULL));
	return (1);
}

static int		editor_init(t_win *win, t_map *map)
{
	map->x = 0;
	map->y = 0;
	map->w = 2000;
	map->h = 1200;
	map->unit = 1.0;
	map->sectors = NULL;
	map->lines = NULL;
	map->rect_util = (SDL_Rect){};
	map->flags = 0;
	ui_init(win);
	return (1);
}

int				editor_loop(t_win *win)
{
	SDL_bool	loop;
	t_map		map;

	// if (!(parser_png("png_test_800_600.png")))
	// 	return (0);
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