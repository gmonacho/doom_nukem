#include "doom_nukem.h"

static void	mouse_move(t_win *win, t_player *player)
{	
	if (win->mouse->x > 0)
	{
		printf("event x>0\n");
		rotate_all(win->map->sectors, player->rz);
	}
	if (win->mouse->x < 0)
	{
		printf("event x<0\n");
		rotate_all(win->map->sectors, player->rz_inv);
	}
	if (win->mouse->y > 0)
	{
		printf("event y>0\n");
		rotate_all(win->map->sectors, player->ry_inv);
	}
	if (win->mouse->y < 0)
	{
		printf("event y<0\n");
		rotate_all(win->map->sectors, player->ry);
	}

}

static void	keyboard_dir(t_win *win, t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_LEFT])
		rotate_all(win->map->sectors, player->rz_inv);
	if (state[SDL_SCANCODE_RIGHT])
		rotate_all(win->map->sectors, player->rz);
	if (state[SDL_SCANCODE_UP])
		rotate_all(win->map->sectors, player->ry);
	if (state[SDL_SCANCODE_DOWN])
		rotate_all(win->map->sectors, player->ry_inv);
	if (state[SDL_SCANCODE_LSHIFT])
		translate_all(win->map->sectors, (t_fdot_3d){0, 0, -1});
	if (state[SDL_SCANCODE_SPACE])
		translate_all(win->map->sectors, (t_fdot_3d){0, 0, 1});
	/*if (state[SDL_SCANCODE_LCTRL])
		player->z -= 4;*/
	if (state[SDL_SCANCODE_KP_MINUS])
		player->fov += -0.03 + (player->fov - 0.03 < 0 ? _2_PI : 0);
	if (state[SDL_SCANCODE_KP_PLUS])
		player->fov +=  0.03 - (player->fov + 0.03 > _2_PI ? _2_PI : 0);
}

static void	keyboard_move(t_win *win, t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_W])
	{	
		translate_all(win->map->sectors, (t_fdot_3d){1, 0, 0});
		// player->vel.x += cos(player->dir) * player->const_vel;
		// player->vel.y += sin(player->dir) * player->const_vel;
	}
	if (state[SDL_SCANCODE_S])
	{
		translate_all(win->map->sectors, (t_fdot_3d){-1, 0, 0});
		// player->vel.x += cos(player->dir + M_PI) * player->const_vel;
		// player->vel.y += sin(player->dir + M_PI) * player->const_vel;
	}
	if (state[SDL_SCANCODE_A])
	{
		translate_all(win->map->sectors, (t_fdot_3d){0, -1, 0});
		// player->vel.x += cos(player->dir - M_PI_2) * player->const_vel;
		// player->vel.y += sin(player->dir - M_PI_2) * player->const_vel;
		// set_origin_rays(player->rays, player->pos_up);
	}
	if (state[SDL_SCANCODE_D])
	{
		translate_all(win->map->sectors, (t_fdot_3d){0, 1, 0});
		// player->vel.x += cos(player->dir + M_PI_2) * player->const_vel;
		// player->vel.y += sin(player->dir + M_PI_2) * player->const_vel;
		// set_origin_rays(player->rays, player->pos_up);
	}
	if (state[SDL_SCANCODE_L])
	{
		player->damage = 5;
	}
}
 static void keyboard_shot(t_win *win, t_player *player, const Uint8 *state, t_music *music)
{	
	char        *tmp;
    SDL_Texture *text;

	tmp = NULL;
	if (state[SDL_SCANCODE_R])
	{	
		if (player->inventory->magazine <= 0)
		{
			tmp = ft_strdup("NO AMMO LEFT");
    		text = generate_text(win->rend, win->texHud->police, tmp, (SDL_Color){200, 0, 2, 40});
    		SDL_RenderCopy(win->rend, text, NULL, &(SDL_Rect){(win->w * 0.05), (win->h * 0.8125), (win->w * 0.25), (win->h * 0.0625)});
		}
		else if (player->inventory->weapon == 1 && player->inventory->ammo < 30)
		{	
			player->inventory->weapon = 2;
			player->timers.reload_cd.index = 0;
            Mix_PlayChannel(3, music->tmusic[1], 0);
			reload_ammo(player);
		}
	}
	if (state[SDL_SCANCODE_I])
        player->inventory->item[0]->nb += 1;
	if (state[SDL_SCANCODE_O])
        player->inventory->item[1]->nb += 1;
	if (state[SDL_SCANCODE_P])
        player->inventory->item[2]->nb += 1;
	if (state[SDL_SCANCODE_E])
	{
		if (test_timer(&(player->timers.item_cd)) == 1)
			use_item(player, music, player->inventory->selected_slot);
	}
	free(tmp);
}

void 		mouse_state(t_win *win, t_player *player, SDL_Event event, t_music *music)
{	
	SDL_Texture     *text;
    char            *tmp;

	if (player->currentHp > 0)
	{	
		tmp = ft_strdup("EMPTY AMMO PRESS 'R' ");
		if (event.motion.xrel || event.motion.yrel)
		{
			mouse_move(win, player);
			player->timers.mouse.index = 1;
		}
		if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				SDL_GetWindowSize(win->ptr, &win->w, &win->h);
				update_ui_rect(win);
				define_line_shot(win, player);
			}
		if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT))
		{	
			if (player->inventory->ammo == 0)
			{
				text = generate_text(win->rend, win->texHud->police, tmp, (SDL_Color){200, 0, 2, 40});
				SDL_RenderCopy(win->rend, text, NULL, &(SDL_Rect){(win->w * 0.33), (win->h * 0.8125), (win->w * 0.28), (win->h * 0.0625)});
			}
			if (test_timer(&(player->timers.bullet_cd)) == 1 && player->inventory->ammo > 0 && player->inventory->weapon == 1)
			{	
				player->timers.bullet_cd.index = 0;
				Mix_PlayChannel(2, music->tmusic[0], 0);
				//Mix_PlayMusic(music->tmusic[0], 1);
				player->inventory->ammo -= 1;
				add_bullet(player);
			}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
		}
		if (event.type == SDL_MOUSEWHEEL && event.wheel.y > 0 && player->inventory->selected_slot != 3 && test_timer(&(player->timers.item_cd)) == 1)
		{
			player->inventory->selected_slot += 1;
			Mix_PlayChannel(1, music->tmusic[4], 0);
			//Mix_PlayMusic(music->tmusic[4], 1);
		}
		if (event.type == SDL_MOUSEWHEEL && event.wheel.y < 0 && player->inventory->selected_slot != 0 && test_timer(&(player->timers.item_cd)) == 1)
		{
			player->inventory->selected_slot -= 1;
			Mix_PlayChannel(1, music->tmusic[4], 0);
			// Mix_PlayMusic(music->tmusic[4], 1);
		}
		free(tmp);
	}
}

int			keyboard_state(t_win *win, t_player *player, t_music *music)
{
	const Uint8	*state;

	if (player->currentHp > 0)
	{
		state = SDL_GetKeyboardState(NULL);
		player->vel = (t_fvector){0, 0};
		keyboard_move(win, player, state);
		
		keyboard_dir(win, player, state);
		keyboard_shot(win, player, state, music);
		//mouse_move(player);
		//printf("Vel : %f\t%f\n", player->vel.x, player->vel.y);
	}
	return (0);
}
