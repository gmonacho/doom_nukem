#include "doom_nukem.h"

	// clock_t			t1;
	// clock_t			t2;
	// t1 = clock();
	// t2 = clock();
	// printf("find coord %lf\n", ((float)t2 - t1) / (float)CLOCKS_PER_SEC);

// static void gravity(t_map *map)
// {
// 	t_poly      *poly_collide;

// 	//Gravity all entities : Player, Mobs, Objects, ...
// 	copy_poly_lst(map->polys_save, map->polys);
// 	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, map->gravity});
// 	while ((poly_collide = collisions(&(map->player), map->polys)))
// 	{
// 		// copy_poly_lst(map->polys, map->polys_save);
// 		slide(map, map->polys, map->polys_save, poly_collide, 0);
// 	}
// }

static SDL_bool game(t_win *win, t_map *map)
{
	t_poly      *poly_collide;
	t_poly      *poly;
	const Uint8 *state;
	SDL_Event   event;
	int            i;

	map->player.debug = 0;
	// for (int i = 0; i < 10000000; i++);
	// printf("Clock : %f\n", (float)clock() / CLOCKS_PER_SEC);
	SDL_GetWindowSize(win->ptr, &win->w, &win->h);

	SDL_PollEvent(&event);
	map->event = &event;
	SDL_GetRelativeMouseState(&(win->winui->mouse.pos.x), &(win->winui->mouse.pos.y));
	state = SDL_GetKeyboardState(NULL);
	// mouse_refresh();

    events_rotate(win, map, &(map->player), state);
    events_actions(win, map, &(map->player), state);
    events_others(win, &(map->player), state);

    copy_poly_lst(map->polys_save, map->polys);
    events_move(win, &(map->player), state);

    if (map->player.collision_on)
    {
        i = 0;
        while ((poly_collide = collisions_sphere(map, &(map->player), map->polys)))
        {
			printf("Col game loop\n");
			poly = map->polys;
			while (poly)
			{
				printf("Is col : %d\n", poly_collision(&(map->player), poly));
				poly = poly->next;
			}
            // printf("c\n");
            // copy_poly_lst(map->polys, map->polys_save);                 //Collision sans slide
            slide(map, map->polys, map->polys_save, poly_collide, i);   //Collision avec slide
            if (i++ == 9)
            {
                printf("Collision avec 10 murs ??\n");
                print_poly(poly_collide, 1);
                printf("%f %f %f %f\n", poly_collide->equation_rotz_only.v.x, poly_collide->equation_rotz_only.v.y, poly_collide->equation_rotz_only.v.z, poly_collide->equation_rotz_only.d);
                exit(0);
            }
        }
        // printf("Col avant grav %p\n", collisions(&(map->player), map->polys));
        // gravity(map);
        // printf("Col apres grav %p\n", collisions(&(map->player), map->polys));
        // printf("\n\n");
    }
	// exit(0);
	printf("Avant rot : %p\n", collisions_sphere(map, &(map->player), map->polys));
    copy_rotate_rotz_only(map->polys, create_ry_matrix(-map->player.rot_y));
	printf("Apres rot : %p\n", collisions_sphere(map, &(map->player), map->polys));
    // printf("Col avant 7 %p\n", collisions(&(map->player), map->polys));


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
		init_main_menu(win);
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
