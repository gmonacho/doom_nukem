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

static void	keyboard_dir(t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_LEFT])
		player->dir -= 0.05;
	if (state[SDL_SCANCODE_RIGHT])
		player->dir += 0.05;
	if (state[SDL_SCANCODE_UP])
		player->orientation += 0.01;
	if (state[SDL_SCANCODE_DOWN])
		player->orientation -= 0.01;
	if (state[SDL_SCANCODE_KP_MINUS])
		player->fov -= 0.03;
	if (state[SDL_SCANCODE_KP_PLUS])
		player->fov += 0.03;

	/*if (player->dir > 2 * M_PI)
		player->dir -= 2 * M_PI; 
	else if (player->dir < 0)
		player->dir += 2 * M_PI;

	if (player->orientation > 2 * M_PI)
		player->orientation -= 2 * M_PI;
	else if (player->orientation < 0)
		player->orientation += 2 * M_PI;*/
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

int			keyboard_state(t_player *player)
{
	const Uint8	*state;

	state = SDL_GetKeyboardState(NULL);
	player->vel = (t_fvector){0, 0};
	keyboard_move(player, state);
	keyboard_dir(player, state);
	//printf("Vel : %f\t%f\n", player->vel.x, player->vel.y);
	//mouse_move(player);
	return (0);
}
