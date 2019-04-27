#include "loop.h"
#include "display.h"
#include "event.h"
#include "data.h"
#include "ret_error.h"

int		game_loop(t_win *win, t_map *map)
{
	SDL_Event	event;
	SDL_bool    loop;

	loop = SDL_TRUE;
	if (win)
	{
		while (loop)
		{
			draw_fps();
			clear_rend(win->rend, 255, 255, 255);
			SDL_PumpEvents();
			SDL_PollEvent(&event);
			move(map, &(map->player));
			if (event.type == SDL_QUIT)
				loop = SDL_FALSE;
			SDL_RenderPresent(win->rend);
		}
	}
	return (SUCCESS);
}
