#include "doom_nukem.h"

/*static void	mouse_move(t_player *player)
{
	static t_dot	lastpos = (t_dot){-100, -100};
	t_dot			newpos;

	SDL_GetMouseState(&newpos.x, &newpos.y);
	if (!(lastpos.x == -100 && lastpos.y == -100))
	{
		if (newpos.x != lastpos.x)
		{
			player->dir += (newpos.x - lastpos.x) / (double)400;
			//printf("Dir : %f\n", player->dir);
		}
		if (newpos.y != lastpos.y)
		{
			//printf("dy\n");
			player->orientation -= (newpos.y - lastpos.y) / (double)10;
		}
	}
	lastpos = (t_dot){newpos.x, newpos.y};
}*/

static void	keyboard_dir(t_win *win, t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_LEFT])
		player->dir += -0.1 + (player->dir - 0.1 < 0 ? _2_PI : 0);
	if (state[SDL_SCANCODE_RIGHT])
		player->dir +=	0.1 - (player->dir + 0.1 > _2_PI ? _2_PI : 0);
	if (state[SDL_SCANCODE_DOWN] && player->orientation > 0)
		player->orientation -= 10;
	if (state[SDL_SCANCODE_LSHIFT])
		player->shift = 1;
	else
		player->shift = 0;
	if (state[SDL_SCANCODE_UP] && player->orientation < win->h)
		player->orientation += 10;
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
}
 static void keyboard_shot(t_win *win, t_player *player, const Uint8 *state)
{	
	char        *tmp;
    SDL_Texture *text;
	int flag;

	tmp = NULL;
	flag = 0;
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
			use_item(player, player->inventory->selected_slot);
	}
	free(tmp);
}

void 		mouse_state(t_win *win, t_player *player, SDL_Event event)
{	
	SDL_Texture     *text;
    char            *tmp;

	tmp = ft_strdup("EMPTY AMMO PRESS 'R' ");
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
			player->inventory->ammo -= 1;
			add_bullet(player);
		}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
	}
	if (event.type == SDL_MOUSEWHEEL && event.wheel.y > 0 && player->inventory->selected_slot != 3 && test_timer(&(player->timers.item_cd)) == 1)
		player->inventory->selected_slot += 1;
	if (event.type == SDL_MOUSEWHEEL && event.wheel.y < 0 && player->inventory->selected_slot != 0 && test_timer(&(player->timers.item_cd)) == 1)
		player->inventory->selected_slot -= 1;
	if (event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			SDL_GetWindowSize(win->ptr, &win->w, &win->h);
			update_ui_rect(win);
			define_line_shot(win, player);
		}
	free(tmp);
}

int			keyboard_state(t_win *win, t_player *player)
{
	const Uint8	*state;

	state = SDL_GetKeyboardState(NULL);
	player->vel = (t_fvector){0, 0};
	keyboard_move(player, state);
	keyboard_dir(win, player, state);
	keyboard_shot(win, player, state);
	//mouse_move(player);
	//printf("Vel : %f\t%f\n", player->vel.x, player->vel.y);
	return (0);
}
