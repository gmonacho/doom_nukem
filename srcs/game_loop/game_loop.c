#include "doom_nukem.h"

int		game_loop(t_win *win, t_map *map)
{
	SDL_Event	event;
	SDL_bool    loop;

	loop = SDL_TRUE;
	if (win)
		while (loop)
		{
			clear_rend(win->rend, 0, 0, 0);
			SDL_PumpEvents();
			SDL_PollEvent(&event);
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
				loop = SDL_FALSE;
			keyboard_state(&(map->player));

			move(map, &(map->player));
			//entiteMove();
			//orientation();	//Haut bas

			draw(win, map, &(map->player));
			SDL_RenderPresent(win->rend);
		}
	return (1);
}
