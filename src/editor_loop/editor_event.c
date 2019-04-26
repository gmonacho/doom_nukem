#include "struct_2d.h"
#include "display.h"
#include "data.h"
#include "loop.h"

int			editor_event(t_win *win, t_map *map, SDL_bool *loop)
{
	SDL_Event	event;

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
		if (!map_add_line(map, new_linedef((t_line){(t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit}, (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit}}, NULL, NULL, 0)))
			return (ret_perror("error map_add_line in editor_event"));
	}
	else if (win->mouse->button[MOUSE_LEFT].releasing)
		map->lines[map->nb_lines - 1].p2 = (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit};
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