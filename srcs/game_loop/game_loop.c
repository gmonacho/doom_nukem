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

    copy_poly_lst(map->polys_save, map->polys);
    printf("1 d0 x %f %f\n", map->polys->next->dots[0].x, map->polys->next->dots_rotz_only[0].x);

    SDL_GetRelativeMouseState(&(win->mouse->x), &(win->mouse->y));
    mouse_state(win, &(map->player), *event, music);
    keyboard_state(win, &(map->player), music);
    copy_rotate_rotz_only(map->polys, create_ry_matrix(-map->player.rot_y));
    
    printf("2 d0 x %f %f\n", map->polys->next->dots[0].x, map->polys->next->dots_rotz_only[0].x);
    if (map->player.collision_on)
    {
        if (collisions(&(map->player), map->polys))        
        {
            printf("Collision : 1\n");
            copy_poly_lst(map->polys, map->polys_save);
        }
        else
            printf("Collision : 0\n");
        printf("3 d0 x %f %f\n\n", map->polys->next->dots[0].x, map->polys->next->dots_rotz_only[0].x);
    }

    clear_rend(win->rend, 0x40, 0x40, 0x40);
    raycasting_3d(win, &(map->player));
    hud(win, &(map->player), win->texHud);
    print_content_slot(win, &(map->player), win->texHud);    
    damage_heal(&(map->player), music, 0, 0);
    SDL_RenderPresent(win->rend);
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
