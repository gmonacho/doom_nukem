#include "loop.h"
#include "display.h"
#include "event.h"
#include "data.h"

int             game_loop(t_win *win)
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
			if (event.type == SDL_QUIT)
				loop = SDL_FALSE;
			SDL_RenderPresent(win->rend);
		}
	}
	return (1);
}
