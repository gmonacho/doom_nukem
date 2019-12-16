#include "doom_nukem.h"
    // clock_t			t1;
	// clock_t			t2;
    // t1 = clock();
	// t2 = clock();
	// printf("find coord %lf\n", ((float)t2 - t1) / (float)CLOCKS_PER_SEC);

static void game(t_win *win, t_map *map, SDL_Event *event, t_music *music)
{

    reload_cd(map);
    SDL_GetWindowSize(win->ptr, &win->w, &win->h);
    clear_rend(win->rend, 0x40, 0x40, 0x40);
    // SDL_PollEvent(event);
    // physics(win, map, &(map->player));
    //entiteMove();

    raycasting_3d(win, &(map->player));
    hud(win, &(map->player), win->texHud);
    mouse_state(win, &(map->player), *event, music);
    keyboard_state(win, &(map->player), music);
    print_content_slot(win, &(map->player), win->texHud);
    
    SDL_GetRelativeMouseState(&(win->mouse->x), &(win->mouse->y));
    //printf("xrel = %d \t | yrel = %d \n", event.motion.xrel, event.motion.yrel);∂∂∂
    damage_heal(&(map->player), music, 0, 0);
    // SDL_RenderPresent(win->rend);
}

int		game_loop(t_win *win, t_map *map)
{
	SDL_Event	event;
	SDL_bool    loop;
	t_music 	*music;

	map->player.currentHp = 100;
	main_inventory(win, &(map->player));
	music = define_music();
	//init_cd(map);
	// printf("Debut game loop\n");
	SDL_SetRelativeMouseMode(SDL_TRUE);
	define_line_shot(win, &(map->player));
    Mix_AllocateChannels(10);
	loop = SDL_TRUE;
    while (loop)
    {
        SDL_PollEvent(&event);
        game(win, map, &event, music);
        if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE || dead_moment(win, &(map->player), win->texHud, event) == 2)
        {
            loop = SDL_FALSE;
            main_menu(win);
        }
        if (dead_moment(win, &(map->player), win->texHud, event) == 1)
        {
            SDL_DestroyWindow(win->ptr);
            SDL_DestroyRenderer(win->rend);
            SDL_Quit();
            exit(0);
        }
        event.type = SDL_FALSE;
    }
	return (1);
}
