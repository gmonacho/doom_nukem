#include "SDL.h"
#include "display.h"
#include "event.h"

int 			create_window(t_win *win, const char *title, SDL_Rect rect, Uint32 flags)
{
	if (!(win->ptr = SDL_CreateWindow(title, rect.x, rect.y, rect.w, rect.h, flags)))
		return (ret_error(SDL_GetError()));
	win->w = rect.w;
	win->h = rect.h;
	if (!(win->rend = SDL_CreateRenderer(win->ptr, 0, SDL_RENDERER_SOFTWARE)))
		return (ret_error(SDL_GetError()));
	SDL_SetRenderDrawBlendMode(win->rend, SDL_BLENDMODE_BLEND);
	return (1);
}

int             game_loop(t_win *win)
{
	SDL_Event	event;
	int     	loop;

	loop = 1;
	if (win)
	{
		while (loop)
		{
			draw_fps();
			SDL_SetRenderDrawColor(win->rend, 255, 255, 255, 255);
			SDL_RenderClear(win->rend);
			SDL_PumpEvents();
			SDL_PollEvent(&event);
			if (key_pressed(SDL_SCANCODE_ESCAPE) || event.type == SDL_QUIT)
				loop = 0;
			SDL_SetRenderDrawColor(win->rend, 0, 0, 0, 255);
			draw_line(win, (t_dot){10, 10}, (t_dot){400, 500});
			SDL_RenderPresent(win->rend);
		}
	}
	return (1);
}