#include "doom_nukem.h"

/*
**	Pour debugger les matrixs de rotation il faut :
**	- Sauvegarder la rotation sur l'autre axe puis l'annuler
**	- Rotater sur le bon axe
**	- Remettre la sauvegarde de la rotation sur lautre axe
*/

static void		events_weapon(t_win *win, t_map *map, t_player *player, const Uint8 *state)
{
	char        *tmp;
    SDL_Texture *text;

	if (map->event->type == SDL_MOUSEWHEEL && test_timer(&(player->timers.item_cd)) == 1)
	{
		if (map->event->wheel.y > 0 && player->inventory->selected_slot != 3)
			player->inventory->selected_slot += 1;
		if (map->event->wheel.y < 0 && player->inventory->selected_slot != 0)
			player->inventory->selected_slot -= 1;
		Mix_PlayChannel(1, map->music->tmusic[4], 0);
	}
	if (state[SDL_SCANCODE_R])
	{	
		if (player->inventory->magazine <= 0)
		{
			tmp = ft_strdup("NO AMMO LEFT");
    		text = generate_text(win->rend, win->texHud->police, tmp, (SDL_Color){200, 0, 2, 40});
			free(tmp);
    		SDL_RenderCopy(win->rend, text, NULL, &(SDL_Rect){(win->w * 0.05), (win->h * 0.8125), (win->w * 0.25), (win->h * 0.0625)});
		}
		else if (player->inventory->weapon == 1 && player->inventory->ammo < 30)
		{	
			player->inventory->weapon = 2;
			player->timers.reload_cd.index = 0;
            Mix_PlayChannel(3, map->music->tmusic[1], 0);
			reload_ammo(player);
		}
	}
	if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT))
	{	
		if (player->inventory->ammo == 0)
		{
			tmp = ft_strdup("EMPTY AMMO PRESS 'R' ");
			text = generate_text(win->rend, win->texHud->police, tmp, (SDL_Color){200, 0, 2, 40});
			free(tmp);
			SDL_RenderCopy(win->rend, text, NULL, &(SDL_Rect){(win->w * 0.33), (win->h * 0.8125), (win->w * 0.28), (win->h * 0.0625)});
		}
		if (test_timer(&(player->timers.bullet_cd)) == 1 && player->inventory->ammo > 0 && player->inventory->weapon == 1)
		{	
			player->timers.bullet_cd.index = 0;
			Mix_PlayChannel(2, map->music->tmusic[0], 0);
			//Mix_PlayMusic(music->tmusic[0], 1);
			player->inventory->ammo -= 1;
			add_bullet(player);
		}
	}
}

void			events_move(t_win *win, t_player *player, const Uint8 *state)
{	
	if (state[SDL_SCANCODE_W])
		translate_all_rotz_only(win->map->polys, (t_fdot_3d){-player->const_vel, 0, 0});
	if (state[SDL_SCANCODE_S])
		translate_all_rotz_only(win->map->polys, (t_fdot_3d){player->const_vel, 0, 0});
	if (state[SDL_SCANCODE_A])
		translate_all_rotz_only(win->map->polys, (t_fdot_3d){0, player->const_vel, 0});
	if (state[SDL_SCANCODE_D])
		translate_all_rotz_only(win->map->polys, (t_fdot_3d){0, -player->const_vel, 0});
	if (state[SDL_SCANCODE_LSHIFT])
		translate_all_rotz_only(win->map->polys, (t_fdot_3d){0, 0, player->const_vel});
	if (state[SDL_SCANCODE_SPACE]/* && printf("onfloor %d\n", player->on_floor) && player->on_floor*/)
		translate_all_rotz_only(win->map->polys, (t_fdot_3d){0, 0, -player->const_vel});
}

void			events_rotate(t_win *win, t_map *map, t_player *player, const Uint8 *state)
{	
	if (map->event->motion.xrel || map->event->motion.yrel)
	{
		if (win->mouse->x > 0)
			rotate_all_rotz_only(win->map->polys, player->rz);
		if (win->mouse->x < 0)
			rotate_all_rotz_only(win->map->polys, player->rz_inv);
		if (win->mouse->y > 0)
			player->rot_y -= player->ddir;
		if (win->mouse->y < 0)
			player->rot_y += player->ddir;
		}
		if (state[SDL_SCANCODE_RIGHT])
			rotate_all_rotz_only(win->map->polys, player->rz);
		if (state[SDL_SCANCODE_LEFT])
			rotate_all_rotz_only(win->map->polys, player->rz_inv);
		if (state[SDL_SCANCODE_DOWN])
			player->rot_y -= player->ddir;
		if (state[SDL_SCANCODE_UP])
			player->rot_y += player->ddir;
		win = (t_win *)state;
		win = (t_win *)map;
}

void			events_actions(t_win *win, t_map *map, t_player *player, const Uint8 *state)
{	
	events_weapon(win, map, player, state);
	if (state[SDL_SCANCODE_I])
		player->inventory->item[0]->nb += 1;
	if (state[SDL_SCANCODE_O])
		player->inventory->item[1]->nb += 1;
	if (state[SDL_SCANCODE_P])
		player->inventory->item[2]->nb += 1;
	if (state[SDL_SCANCODE_E])
		if (test_timer(&(player->timers.item_cd)) == 1)
			use_item(player, map->music, player->inventory->selected_slot);
	if (state[SDL_SCANCODE_L])
		player->damage = 5;
}

void			events_others(t_win *win, t_player *player, const Uint8 *state)
{
	t_poly		*poly;

	if (state[SDL_SCANCODE_KP_MINUS])
		player->fov += -0.03 + (player->fov - 0.03 < 0 ? _2_PI : 0);
	if (state[SDL_SCANCODE_KP_PLUS])
		player->fov +=  0.03 - (player->fov + 0.03 > _2_PI ? _2_PI : 0);
	if (state[SDL_SCANCODE_1])
		win->view += (win->view & TEXTURE_VIEW ? -TEXTURE_VIEW : TEXTURE_VIEW);
	if (state[SDL_SCANCODE_2])
		win->view += (win->view & WALL_VIEW ? -WALL_VIEW : WALL_VIEW);
	if (state[SDL_SCANCODE_3])
		win->view += (win->view & BOX_VIEW ? -BOX_VIEW : BOX_VIEW);
	if (state[SDL_SCANCODE_C])
		player->collision_on = player->collision_on ? 0 : 1;
	if (state[SDL_SCANCODE_P])
	{
		poly = win->map->polys;
		while (poly)
		{
			printf("Equation %f x + %f y + %f z + %f = 0\n", poly->equation.v.x, poly->equation.v.y, poly->equation.v.z, poly->equation.d);
			print_poly(poly, 0);
			printf("I %f %f %f\n", poly->i.x, poly->i.y, poly->i.z);
			printf("J %f %f %f\n", poly->j.x, poly->j.y, poly->j.z);
			printf("II JJ IJ IJIJ_IIJJ %f %f %f %f\n", poly->ii, poly->jj, poly->ij, poly->ijij_iijj);
			poly = poly->next;
		}
		printf("\n\n");
	}
}

// void 		mouse_state(t_win *win, t_player *player, SDL_Event event, t_music *music)
// {
// 	if (event.motion.xrel || event.motion.yrel)
// 	{
// 		mouse_move(win, player);
// 		player->timers.mouse.index = 1;
// 	}
// 	if (event.window.event == SDL_WINDOWEVENT_RESIZED)
// 	{
// 		SDL_GetWindowSize(win->ptr, &win->w, &win->h);
// 		update_ui_rect(win);
// 		define_line_shot(win, player);
// 	}
// }

