#include "display.h"
#include "data.h"
#include "loop.h"
#include "struct_2d.h"

int			editor_event(t_win *win, t_map *map, SDL_bool *loop)
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
	else if(event.type == SDL_MOUSEWHEEL)
	{
		if (event.wheel.y > 0)
			map_zoom(map, 0.05);
		else if (event.wheel.y < 0)
			map_zoom(map, -0.05);
	}
	if (win->mouse->button[MOUSE_LEFT].pressing)
	{
		dot = is_next_to_linedef(&map->lines, (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit}, SDL_FALSE, map->unit * 20);
		if (!(tmp = new_linedef((t_line){dot, dot}, NULL, NULL, LINEDEF_NONE)))
			return (0);
		add_linedef(&map->lines, tmp);
		map->flags = DRAWING_LINE;
	}
	else if (win->mouse->button[MOUSE_LEFT].releasing)
	{
		map->lines->p2 = is_next_to_linedef(&map->lines, (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit}, SDL_TRUE, map->unit * 20);

	}
	if (win->mouse->button[MOUSE_RIGHT].pressing)
	{
		map->rect_util = (SDL_Rect){win->mouse->x, win->mouse->y, 0, 0};
	}
	else if (win->mouse->button[MOUSE_RIGHT].releasing)
	{
		selected_linedef(map, LINEDEF_SELECTED);
		map->rect_util = (SDL_Rect){};
	}
	if (win->mouse->button[MOUSE_LEFT].pressed)
	{
		map->lines->p2 = is_next_to_linedef(&map->lines, (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit}, SDL_TRUE, map->unit * 20);
		if (map->lines->p2.x != (win->mouse->x - map->x) / map->unit || map->lines->p2.y != (win->mouse->y - map->y) / map->unit) // PAS BON
		{
			if (is_line_vertical(map->lines->p1.x, map->lines->p2.x, map->unit * 20))
				map->lines->p2.x = map->lines->p1.x;
			if (is_line_horizontal(map->lines->p1.y, map->lines->p2.y, map->unit * 20))
				map->lines->p2.y = map->lines->p1.y;
		}
		map->flags = 0;
	}
	if (win->mouse->button[MOUSE_RIGHT].pressed)
	{
		map->rect_util.w = win->mouse->x - map->rect_util.x;
		map->rect_util.h = win->mouse->y - map->rect_util.y;
		selected_linedef(map, LINEDEF_MOUSE_POINTED);
	}
	if (key_pressed(SDL_SCANCODE_DELETE))
		delete_linedef(&map->lines, LINEDEF_SELECTED);
	if (key_pressed(SDL_SCANCODE_W))
		map->y += 1;
	if (key_pressed(SDL_SCANCODE_S))
		map->y -= 1;
	if (key_pressed(SDL_SCANCODE_A))
		map->x += 1;
	if (key_pressed(SDL_SCANCODE_D))
		map->x -= 1;
	return (1);
}