#include "doom_nukem.h"


// static void	mouse_move(t_win *win ,t_player *player, int vx, int vy)
// {
// 	if (vx > 0)
// 		player->dir += (double)vx / 800 + ((player->dir + (double)vx / 800 > 2 * M_PI) ? -2 * M_PI : 0);
// 	if (vx < 0)
// 		player->dir += (double)vx / 800 + ((player->dir - (double)vx / 800 < 0) ? 2 * M_PI : 0);

// 	if (vy > 0 && player->orientation > 15)
// 		player->orientation -= 10 * (double)vy / 10;
// 	if (vy < 0 && player->orientation < win->h - 15)
// 		player->orientation -= 10 * (double)vy / 10;
// }
// static void	mouse_move(t_win *win ,t_player *player)
// {
// 	static t_dot	lastpos = (t_dot){-100, -100};
// 	t_dot			newpos;
// 	// float x;
// 	// float y;

// 	SDL_GetMouseState(&newpos.x, &newpos.y);
// 	printf("x = %d | y = %d\n", newpos.x , newpos.y);
// 	// x = (newpos.x - lastpos.x) / 1000;
// 	// y = (newpos.y - lastpos.y) / 1000;
// 	if (!(lastpos.x == -100 && lastpos.y == -100))
// 	{
// 		if (newpos.x > lastpos.x)
// 			player->dir += 0.1 + ((player->dir + 0.1 > 2 * M_PI) ? -2 * M_PI : 0);
// 		else if (newpos.x < lastpos.x)
// 			player->dir -= 0.1 + ((player->dir - 0.1 < 0) ? 2 * M_PI : 0);

// 		if (newpos.y > lastpos.y)
// 			player->orientation -= 15;
// 		else if (newpos.y < lastpos.y && player->orientation < win->h)
// 			player->orientation += 15;

// 		// printf("orientation = %f\n", player->orientation);
// 		// printf("x = %f | y = %f\n", x ,y);
// 	}
// 	lastpos = (t_dot){newpos.x, newpos.y};
// }

static void	mouse_move(t_win *win ,t_player *player)
{	
	if (win->mouse->x > 0)
		player->dir += 0.1 + ((player->dir + 0.1 > 2 * M_PI) ? -2 * M_PI : 0);
	if (win->mouse->x < 0)
		player->dir -= 0.1 + ((player->dir - 0.1 < 0) ? 2 * M_PI : 0);
	// if (win->mouse->y > 0 && player->dir_up > 15)
	// 	player->dir_up -= 15;
	// if (win->mouse->y < 0 && player->dir_up < win->h - 15)
	// 	player->dir_up += 15;
	if (win->mouse->y > 0)
		player->dir_up -= 0.1 + ((player->dir_up - 0.1 < 0) ? 2 * M_PI : 0);
	if (win->mouse->y < 0)
		player->dir_up += 0.1 + ((player->dir_up + 0.1 > 2 * M_PI) ? -2 * M_PI : 0);
}

static void	keyboard_dir(t_win *win, t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_LEFT])
		player->dir += -0.1 + (player->dir - 0.1 < 0 ? _2_PI : 0);
	if (state[SDL_SCANCODE_RIGHT])
		player->dir +=	0.1 - (player->dir + 0.1 > _2_PI ? _2_PI : 0);
	if (state[SDL_SCANCODE_DOWN] && player->dir_up > 0)
		player->dir_up -= 10;
	if (state[SDL_SCANCODE_LSHIFT])
		player->shift = 1;
	else
		player->shift = 0;
	if (state[SDL_SCANCODE_UP] && player->dir_up < win->h)
		player->dir_up += 10;
	if (state[SDL_SCANCODE_SPACE] && player->jump)
		player->z += 9;
	/*if (state[SDL_SCANCODE_LCTRL])
		player->z -= 4;*/
	if (state[SDL_SCANCODE_KP_MINUS])
		player->fov += -0.03 + (player->fov - 0.03 < 0 ? _2_PI : 0);
	if (state[SDL_SCANCODE_KP_PLUS])
		player->fov +=  0.03 - (player->fov + 0.03 > _2_PI ? _2_PI : 0);
}

static void	keyboard_move(t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_W])
	{	
		player->vel.x += cos(player->dir) * player->const_vel;
		player->vel.y += sin(player->dir) * player->const_vel;
	}
	if (state[SDL_SCANCODE_S])
	{
		player->vel.x += cos(player->dir + M_PI) * player->const_vel;
		player->vel.y += sin(player->dir + M_PI) * player->const_vel;
	}
	if (state[SDL_SCANCODE_A])
	{
		player->vel.x += cos(player->dir - M_PI_2) * player->const_vel;
		player->vel.y += sin(player->dir - M_PI_2) * player->const_vel;
	}
	if (state[SDL_SCANCODE_D])
	{
		player->vel.x += cos(player->dir + M_PI_2) * player->const_vel;
		player->vel.y += sin(player->dir + M_PI_2) * player->const_vel;
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
		keyboard_move(player, state);
		
		keyboard_dir(win, player, state);
		keyboard_shot(win, player, state, music);
		//mouse_move(player);
		//printf("Vel : %f\t%f\n", player->vel.x, player->vel.y);
	}
	return (0);
}
