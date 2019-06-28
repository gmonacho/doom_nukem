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
		player->dir -= 0.1;
	if (state[SDL_SCANCODE_RIGHT])
		player->dir += 0.1;
	if (state[SDL_SCANCODE_DOWN] && player->orientation > 0)
		player->orientation -= 10;
	if (state[SDL_SCANCODE_UP] && player->orientation < win->h)
		player->orientation += 10;
	if (state[SDL_SCANCODE_LSHIFT])
		player->shift = 1;
	else
		player->shift = 0;
	if (state[SDL_SCANCODE_SPACE] && player->jump)
		player->z += 9;
	/*if (state[SDL_SCANCODE_LCTRL])
		player->z -= 4;*/
	if (state[SDL_SCANCODE_KP_MINUS])
		player->fov -= 0.03;
	if (state[SDL_SCANCODE_KP_PLUS])
		player->fov += 0.03;
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

/*static void keyboard_hud(t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_K] && player->currentHp > 0)
		player->damage = 5;
	if (state[SDL_SCANCODE_L] && player->currentHp < 100)
		player->currentHp += 5;
}*/

int			keyboard_state(t_win *win, t_player *player)
{
	const Uint8	*state;

	state = SDL_GetKeyboardState(NULL);
	player->vel = (t_fvector){0, 0};
	keyboard_move(player, state);
	keyboard_dir(win, player, state);
	//keyboard_hud(player, state);
	//mouse_move(player);
	//printf("Vel : %f\t%f\n", player->vel.x, player->vel.y);
	return (0);
}
