#include "display.h"
#include "data.h"
#include "loop.h"
#include "struct_2d.h"
#include "physics.h"

static void		update_selected_ui(t_win *win)
{
	t_frame		*frame_tmp;
	t_button	*button_tmp;

	win->selected_frame = NULL;
	win->selected_button = NULL;
	frame_tmp = win->frames;
	while (frame_tmp)
	{
		if (is_in_rect(frame_tmp->rect, (t_dot){win->mouse->x, win->mouse->y}))
		{
			win->selected_frame = frame_tmp;
			button_tmp = frame_tmp->buttons;
			while (button_tmp)
			{
				if (is_in_rect(button_tmp->rect, (t_dot){win->mouse->x, win->mouse->y}))
				{
					win->selected_button = button_tmp;
					button_tmp = NULL;
					frame_tmp = NULL;
				}
				if (button_tmp)
					button_tmp = button_tmp->next;
			}
		}
		if (frame_tmp)
			frame_tmp = frame_tmp->next;
	}
}

static void		update_ui_rect(t_win *win)
{
	t_frame		*frame_tmp;
	t_button	*button_tmp;

	frame_tmp = win->frames;
	while (frame_tmp)
	{
		frame_tmp->rect = (SDL_Rect){win->w * frame_tmp->ratio.x,
										win->h * frame_tmp->ratio.y,
										win->w * frame_tmp->ratio.w,
										win->h * frame_tmp->ratio.h};
		button_tmp = frame_tmp->buttons;
		while (button_tmp)
		{
			button_tmp->rect = (SDL_Rect){button_tmp->ratio.x * frame_tmp->rect.w + frame_tmp->rect.x,
								button_tmp->ratio.y * frame_tmp->rect.h + frame_tmp->rect.y,
								button_tmp->ratio.w * frame_tmp->rect.w,
								button_tmp->ratio.h * frame_tmp->rect.h};
			button_tmp = button_tmp->next;
		}
		frame_tmp = frame_tmp->next;
	}
}

static void		resolve_ui_left_release(t_win *win, t_map *map)
{
	t_frame		*f;

	f = win->selected_frame;
	if (win->selected_frame->flags & FRAME_SECTORS)
	{
		if (win->mouse->x < f->rect.x + (f->rect.w / MAX_SECTORS) * (f->nb_buttons - 1))
			map->selected_sector = (win->mouse->x - f->rect.x) / (f->rect.w / MAX_SECTORS);
		else if (win->mouse->x > f->rect.x + (f->rect.w / MAX_SECTORS) * (f->nb_buttons - 1) &&
				win->mouse->x < f->rect.x + (f->rect.w / MAX_SECTORS) * f->nb_buttons)
		{
			add_button_to_frame(&win->selected_frame, new_button((t_frect){1.0 / MAX_SECTORS * f->nb_buttons, 0, 1.0 / MAX_SECTORS, 1}, NULL, 0));
			add_sector(&map->sectors, new_sector());
		}
	}
}

int				editor_event(t_win *win, t_map *map, SDL_bool *loop)
{
	SDL_Event	event;
	t_linedef	*tmp;
	t_dot		dot;

	tmp = NULL;
	SDL_PumpEvents();
	mouse_refresh();
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
		*loop = SDL_FALSE;
	else if (event.type == SDL_MOUSEMOTION)
	{
		update_selected_ui(win);
	}
	else if(event.type == SDL_MOUSEWHEEL)
	{
		if (event.wheel.y > 0)
			map_zoom(map, 0.05);
		else if (event.wheel.y < 0)
			map_zoom(map, -0.05);
	}
	else if(event.type == SDL_WINDOWEVENT)
	{
		if(event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			SDL_GetWindowSize(win->ptr, &win->w, &win->h);
			update_ui_rect(win);
		}
	}
	if (win->mouse->button[MOUSE_LEFT].pressing)
	{
		if (!win->selected_frame)
		{
			dot = (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit};
			is_next_to_linedef(&map->lines, &dot, map->unit * NEXT_FACTOR);
			if (!(tmp = new_linedef((t_line){dot, dot}, NULL, NULL, LINEDEF_NONE)))
				return (0);
			add_linedef(&map->lines, tmp);
			map->flags = map->flags | DRAWING_LINE;
		}
	}
	else if (win->mouse->button[MOUSE_LEFT].releasing)
	{
		if (win->selected_frame)
		{
			resolve_ui_left_release(win, map);
		}
		else
		{
			if (map->flags & DRAWING_LINE)
			{
				dot = (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit};
				is_next_to_linedef(&map->lines, &dot, map->unit * NEXT_FACTOR);
				map->lines->p2 = dot;
				map->flags -= DRAWING_LINE;
			}
		}
	}

	if (win->mouse->button[MOUSE_RIGHT].pressing)
	{
		if (!win->selected_frame)
		{
			map->rect_util = (SDL_Rect){win->mouse->x, win->mouse->y, 0, 0};
			map->flags = map->flags | MAP_SELECTING;
		}
	}
	else if (win->mouse->button[MOUSE_RIGHT].releasing)
	{
		if (map->flags & MAP_SELECTING)
		{
			selected_linedef(map, LINEDEF_SELECTED);
			map->rect_util = (SDL_Rect){};
			map->flags -= MAP_SELECTING;
		}
	}	
	if (win->mouse->button[MOUSE_MIDDLE].pressing)
		mouse_drag(win->mouse->x, win->mouse->y, SDL_FALSE);

	if (win->mouse->button[MOUSE_LEFT].pressed)
	{
		if (map->flags & DRAWING_LINE)
		{
			map->lines->p2 = (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit};
			is_next_to_linedef(&map->lines, &map->lines->p2, map->unit * NEXT_FACTOR);
		}
	}
	if (win->mouse->button[MOUSE_RIGHT].pressed)
	{
		if (map->flags & MAP_SELECTING)
		{
			map->rect_util.w = win->mouse->x - map->rect_util.x;
			map->rect_util.h = win->mouse->y - map->rect_util.y;
			selected_linedef(map, LINEDEF_MOUSE_POINTED);
		}
	}
	if (win->mouse->button[MOUSE_MIDDLE].pressed)
	{
		dot = mouse_drag(win->mouse->x, win->mouse->y, SDL_TRUE);
		map->x += dot.x;
		map->y += dot.y;
		mouse_drag(win->mouse->x, win->mouse->y, SDL_FALSE);
	}

	if (key_pressed(SDL_SCANCODE_DELETE))
		delete_linedef(&map->lines, LINEDEF_SELECTED);
	if (key_pressed(SDL_SCANCODE_W))
		map->y += 2;
	if (key_pressed(SDL_SCANCODE_S))
		map->y -= 2;
	if (key_pressed(SDL_SCANCODE_A))
		map->x += 2;
	if (key_pressed(SDL_SCANCODE_D))
		map->x -= 2;
	return (1);
}