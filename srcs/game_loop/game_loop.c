#include "doom_nukem.h"

int		game_loop(t_win *win, t_map *map)
{
	SDL_Event	event;
	SDL_bool    loop;
	t_texHud	*texHud;

	texHud = define_texHud(win);
	main_inventory(win, &(map->player));
	start_cooldown(&(map->player.timers.bullet_cd), 100);
	start_cooldown(&(map->player.timers.item_cd), 200);
	start_cooldown(&(map->player.timers.text_cd), 600);
	printf("Debut game loop\n");
	loop = SDL_TRUE;
	if (win)
		while (loop)
		{
			clear_rend(win->rend, 0x40, 0x40, 0x40);
			SDL_PumpEvents();
			SDL_PollEvent(&event);
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
				loop = SDL_FALSE;
			keyboard_state(win, &(map->player));

			move(win, map, &(map->player));
			//entiteMove();

			draw(win, map, &(map->player));
			//damage_heal(&(map->player), 1, 0, 0);
			hud(win, &(map->player), texHud);
            print_content_slot(win, &(map->player), texHud);
			mouse_state(&(map->player), event);
			event.type = SDL_FALSE;
			SDL_RenderPresent(win->rend);
		}
	return (1);
}
