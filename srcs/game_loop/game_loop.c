/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/07 22:13:37 by agiordan          #+#    #+#             */
/*   Updated: 2020/05/17 23:42:25 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static int		end_game(t_win *win, t_map *map,
						t_player *player,
						SDL_Event *event)
{
	int	i;

	if (event->type == SDL_QUIT ||\
			event->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
	{
		init_main_menu(win);
		return (1);
	}
	if (player->current_hp <= 0)
	{
		i = dead_menu(win, player);
		if (i == 2)
		{
			main_free(win, map);
			exit(0);
		}
		else
		{
			init_main_menu(win);
			return (1);
		}
	}
	return (0);
}

static int		tests_before_slide(t_map *map, t_poly *poly_collide,\
																t_fdot_3d move)
{
	t_fdot_3d	poly_collide_vector;

	translate_all_poly_rotz_only(map->polys,\
							(t_fdot_3d){0, 0, map->player._4_height_10});
	if (collision_dots(poly_collide->dots_rotz_only, map->player.width_2))
	{
		translate_all_poly_rotz_only(map->polys,\
							(t_fdot_3d){0, 0, -map->player._4_height_10});
		return (1);
	}
	translate_all_poly_rotz_only(map->polys,\
							(t_fdot_3d){0, 0, -map->player._4_height_10});
	poly_collide_vector = poly_collide->equation_rotz_only.v;
	if (poly_collide->segment_code)
	{
		poly_collide->is_slide_ban = 1;
		poly_collide_vector = segment_slide(poly_collide->dots_rotz_only,\
				poly_collide->equation_rotz_only, poly_collide->segment_code);
	}
	move.x = move.x;
	slide(map, map->polys, map->polys_save, poly_collide_vector);
	return (0);
}

void			move_and_collide(t_win *win, t_map *map, t_player *player, t_fdot_3d move)
{
	t_fdot_3d	move_slide;
	t_poly		*poly_collide;
	int			i;

	copy_poly_lst(map->polys_save, map->polys);
	translate_all_poly_rotz_only(map->polys, move);
	if (!player->collision_on)
		return ;
	i = 0;
	while ((poly_collide = collisions_sphere(map, player, map->polys, 1)))
	{
		if (poly_collide->object && poly_collide->object->type != BOX)
			objects_actions(win, map, player, poly_collide->object);
		if (i++ == 4 ||\
				tests_before_slide(map, poly_collide, move))
			break ;
	}
	if (collisions_sphere(map, player, map->polys, 0))
		copy_poly_lst(map->polys, map->polys_save);
	move_slide = fdot_3d_sub(map->polys->dots_rotz_only[0],\
			map->polys_save->dots_rotz_only[0]);
	map->last_move = fdot_3d_add(map->last_move, move_slide);
	translate_all_objects_rotz_only(map->objects, move_slide);
	map->sky_box.pos_rotz_only = fdot_3d_add(map->sky_box.pos_rotz_only,\
			move_slide);
}

static SDL_bool	game(t_win *win, t_map *map, t_player *player)
{
	SDL_Event	event;

	events_game_loop(win, map, player, &event);
	draw(win, map, player);
	if (end_game(win, map, player, &event))
		return (SDL_FALSE);
	SDL_RenderPresent(win->rend);
	return (SDL_TRUE);
}

int				game_loop(t_win *win, t_map *map)
{
	SDL_bool	loop;

	map->player.current_hp = 100;
	SDL_SetRelativeMouseMode(SDL_TRUE);
	if (!(map->save.if_pars))
	{
		Mix_AllocateChannels(10);
		map->music = define_music();
		main_inventory(win, &(map->player));
		define_line_shot(win, &(map->player));
		map->save.if_pars = 1;
	}
	loop = SDL_TRUE;
	while (loop)
		loop = game(win, map, &(map->player));
	return (1);
}
