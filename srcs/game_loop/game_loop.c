#include "doom_nukem.h"

    // clock_t			t1;
	// clock_t			t2;
    // t1 = clock();
	// t2 = clock();
	// printf("find coord %lf\n", ((float)t2 - t1) / (float)CLOCKS_PER_SEC);

static void gravity(t_map *map)
{
    t_poly      *poly_collide;

    //Gravity all entities : Player, Mobs, Objects, ...
    copy_poly_lst(map->polys_save, map->polys);
    translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, map->gravity});
    while ((poly_collide = collisions(&(map->player), map->polys)))
    {
        // copy_poly_lst(map->polys, map->polys_save);
        slide(map, map->polys, map->polys_save, poly_collide);
    }
}

static SDL_bool game(t_win *win, t_map *map)
{
    t_poly      *poly_collide;
    const Uint8 *state;
    SDL_Event   event;

    // for (int i = 0; i < 10000000; i++);
    // printf("Clock : %f\n", (float)clock() / CLOCKS_PER_SEC);
    SDL_GetWindowSize(win->ptr, &win->w, &win->h);

    SDL_PollEvent(&event);
    map->event = &event;
    SDL_GetRelativeMouseState(&(win->mouse->x), &(win->mouse->y));
	state = SDL_GetKeyboardState(NULL);
    // mouse_refresh();

    events_rotate(win, map, &(map->player), state);
    events_actions(win, map, &(map->player), state);
    events_others(win, &(map->player), state);
    
    copy_poly_lst(map->polys_save, map->polys);
    events_move(win, &(map->player), state);

    if (map->player.collision_on)
    {
        while ((poly_collide = collisions(&(map->player), map->polys)))
        {
            // copy_poly_lst(map->polys, map->polys_save);                 //Collision sans slide
            slide(map, map->polys, map->polys_save, poly_collide);   //Collision avec slide
        }
        gravity(map);
    }
    copy_rotate_rotz_only(map->polys, create_ry_matrix(-map->player.rot_y));

    clear_rend(win->rend, 0x40, 0x40, 0x40);
    raycasting_3d(win, &(map->player));

    reload_cd(map);
    damage_heal(&(map->player), map->music, 0, 0);
    print_content_slot(win, &(map->player), win->texHud);    
    hud(win, &(map->player), win->texHud);
    if (event.type == SDL_QUIT ||\
        event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ||\
        dead_moment(win, &(map->player), win->texHud, event) == 2)
    {
        printf("Main menu\n");
        main_menu(win);
        return (SDL_FALSE);
    }
    if (dead_moment(win, &(map->player), win->texHud, event) == 1)
    {
        printf("Mort\n");
        SDL_DestroyWindow(win->ptr);
        SDL_DestroyRenderer(win->rend);
        SDL_Quit();
        exit(0);
    }
    event.type = SDL_FALSE;
    SDL_RenderPresent(win->rend);
    return (SDL_TRUE);
}

int     		game_loop(t_win *win, t_map *map)
{
    SDL_bool    loop;

	map->player.currentHp = 100;
	map->music = define_music();
	SDL_SetRelativeMouseMode(SDL_TRUE);
    Mix_AllocateChannels(10);
	// init_cd(map);
	main_inventory(win, &(map->player));
	define_line_shot(win, &(map->player));
	loop = SDL_TRUE;
    while (loop)
        loop = game(win, map);
	return (1);
}
