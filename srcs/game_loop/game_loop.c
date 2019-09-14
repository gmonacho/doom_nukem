#include "doom_nukem.h"

int		game_loop(t_win *win, t_map *map)
{
	SDL_Event	event;
	SDL_bool    loop;
	int 		i;

	i = 0;
	win->texHud = define_texHud(win);
	main_inventory(win, &(map->player));
	//init_cd(map);
	printf("Debut game loop\n");
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
			//entiteMove();

			draw(win, map, &(map->player));
			//damage_heal(&(map->player), 1, 0, 0);
			hud(win, &(map->player), win->texHud);
			mouse_state(win, &(map->player), event);
			keyboard_state(win, &(map->player));
            print_content_slot(win, &(map->player), win->texHud);
			//main_animation(map, win, win->texHud);
			event.type = SDL_FALSE;
			SDL_RenderPresent(win->rend);
			//printf("index = %d\n", map->player.timers.reload_cd.index);
		}
	return (1);
}
