#include "doom_nukem.h"


static int		tests_before_slide(t_map *map, t_poly *poly_collide, t_fdot_3d move)
{
	t_fdot_3d	poly_collide_v;

	translate_all_poly_rotz_only(map->polys, (t_fdot_3d){0, 0, map->player._4_height_10});
	if (collision_dots(map, poly_collide->dots_rotz_only, map->player.width_2))
	{
		// printf("Dot is in Poly, return last state\n");
		// copy_poly_lst(map->polys, map->polys_save);                 //Collision sans slide
		translate_all_poly_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
		return (1);
	}
	translate_all_poly_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
	poly_collide_v = poly_collide->equation_rotz_only.v;
	if (poly_collide->segment_code)
	{
		poly_collide->is_slide_ban = 1;
		poly_collide_v = segment_slide(poly_collide->dots_rotz_only, poly_collide->equation_rotz_only, poly_collide->segment_code);
		// printf("Segment code : %d -> %d %d\n", poly_collide->segment_code, poly_collide->segment_code & 0b11, (poly_collide->segment_code & 0b1100) >> 2);
		// printf("Segment -> plan slide : %f %f %f\n", poly_collide_v.x, poly_collide_v.y, poly_collide_v.z);
	}
	move.x = move.x;
	slide(map, map->polys, map->polys_save, poly_collide_v);   //Collision avec slide
	return (0);
}

static void		move_and_collide(t_map *map, t_player *player, t_fdot_3d move)
{
	t_poly      *poly_collide;
	int			i;

	copy_poly_lst(map->polys_save, map->polys);
	translate_all_poly_rotz_only(map->polys, move);
	// translate_all_rotz_only(map, map->polys, move);
	if (!player->collision_on)
		return ;
	i = 0;
	while ((poly_collide = collisions_sphere(map, player, map->polys, 1)))
	{
		// printf("Collision ! Index : %d\n", poly_collide->index);

		// if (poly_collide->object)
		if (poly_collide->object && poly_collide->object->type != BOX)
			if (objects_actions(map, player, poly_collide->object))
				continue ;
		if (i++ == 4 ||\
			tests_before_slide(map, poly_collide, move))
			break ;
	}
	// if (!i)
	// 	translate_all_objects_rotz_only(map->objects, move);
	if (collisions_sphere(map, player, map->polys, 0))
	{
		// printf("Toujours collision ! Return last state\n");
		copy_poly_lst(map->polys, map->polys_save);                 //Collision sans slide               //Collision sans slide
	}
	translate_all_objects_rotz_only(map->objects, fdot_3d_sub(map->polys->dots_rotz_only[0],\
													map->polys_save->dots_rotz_only[0]));
}

static SDL_bool game(t_win *win, t_map *map, t_player *player)
{
	// t_poly      *poly;
	// t_poly      *ret;
	const Uint8 *state;
	SDL_Event   event;
	int			i;
	// clock_t		t1;
	// clock_t		t2;
	// t1 = clock();
	// t2 = clock();
	// printf("Delta time %lf\n", ((float)t2 - t1) / (float)CLOCKS_PER_SEC);

	// player->debug = 0;
	SDL_GetWindowSize(win->ptr, &win->w, &win->h);
	SDL_PollEvent(&event);
	map->event = &event;
	SDL_GetRelativeMouseState(&(win->winui->mouse.pos.x), &(win->winui->mouse.pos.y));
	state = SDL_GetKeyboardState(NULL);
	// mouse_refresh();

	events_rotate(win, map, player, state);
    events_others(win, player, state);

	move_and_collide(map, player, events_move(player, state));
	move_and_collide(map, player, (t_fdot_3d){0, 0, map->gravity});
	mobs_attack_move(map, player, map->mob);

	events_actions(win, map, player, state);
	// copy_rotate_rotz_only(player, map->polys, create_ry_matrix(-player->rot_y));
	copy_rotate_rotz_only(map, map->polys, create_ry_matrix(-player->rot_y));
	
	
	// t_object	*object;
	// object = map->objects;
	// while (object)
	// {
	// 	if (object->type == LIGHT)
	// 		printf("Pos %f %f %f\n", object->pos_rotz_only.x, object->pos_rotz_only.y, object->pos_rotz_only.z);
	// 	// t_fdot_3d mid = fdot_3d_add(object->poly->dots[0], object->poly->dots[2]);
	// 	// printf("Pos %f %f %f\n", object->pos.x, object->pos.y, object->pos.z);
	// 	// printf("Mid %f %f %f\n", mid.x / 2, mid.y / 2, mid.z / 2);
	// 	object = object->next;
	// }
	// printf("\n");


	objects_movements(map, player, map->objects);

	clear_rend(win->rend, 0x40, 0x40, 0x40);
	raycasting_3d(win, player);

	reload_cd(map);
	hud(win, player, win->texhud);
	print_content_slot(win, player, win->texhud);
	if (event.type == SDL_QUIT ||\
		event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
	{	
		init_main_menu(win);
		return (SDL_FALSE);
	}
	if (player->currentHp <= 0)
	{
		i = dead_menu(win, player);
		if (i == 2)
		{	
			printf("Mort\n");
			SDL_DestroyWindow(win->ptr);
			SDL_DestroyRenderer(win->rend);
			SDL_Quit();
			exit(0);
		}
		else
		{
			init_main_menu(win);
			return (SDL_FALSE);
		}
	}
	event.type = SDL_FALSE;
	SDL_RenderPresent(win->rend);
	return (SDL_TRUE);
}

int     		game_loop(t_win *win, t_map *map)
{
	SDL_bool    loop;

	map->player.currentHp = 100;
	SDL_SetRelativeMouseMode(SDL_TRUE);
	// init_cd(map);
	if (!(map->save.ifPars))
	{
		Mix_AllocateChannels(10);
		map->music = define_music();
		main_inventory(win, &(map->player));	
		define_line_shot(win, &(map->player));
		map->save.ifPars = 1;
	}
	loop = SDL_TRUE;
	while (loop)
		loop = game(win, map, &(map->player));
	return (1);
}
