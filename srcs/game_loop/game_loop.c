#include "doom_nukem.h"

int		game_loop(t_win *win, t_map *map)
{
	SDL_Event	event;
	SDL_bool    loop;	

	win->texHud = define_texHud(win);
	main_inventory(win, &(map->player));
	//init_cd(map);
	printf("Debut game loop\n");
	SDL_SetRelativeMouseMode(SDL_TRUE);
	define_line_shot(win, &(map->player));
	loop = SDL_TRUE;
	if (win)
		while (loop)
		{
			reload_cd(map);
			clear_rend(win->rend, 0x40, 0x40, 0x40);
			SDL_PumpEvents();
			SDL_PollEvent(&event);
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
				loop = SDL_FALSE;

			physics(win, map, &(map->player));
			//entiteMove();a

			draw(win, map, &(map->player));
			//damage_heal(&(map->player), 1, 0, 0);
			hud(win, &(map->player), win->texHud);
			mouse_state(win, &(map->player), event);
			keyboard_state(win, &(map->player));
            print_content_slot(win, &(map->player), win->texHud);
			event.type = SDL_FALSE;
			SDL_RenderPresent(win->rend);
			SDL_GetRelativeMouseState(&(win->mouse->x), &(win->mouse->y));
			//printf("xrel = %d \t | yrel = %d \n", event.motion.xrel, event.motion.yrel);
		}
	return (1);
}
