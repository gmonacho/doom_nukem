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

SDL_bool		is_in_screen(t_win *win, t_dot p)
{
	return ((p.x >= 0 && p.x < win->w && p.y >= 0 && p.y < win->h));
}

