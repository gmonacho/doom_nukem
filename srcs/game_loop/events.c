#include "doom_nukem.h"

static void	mouse_move(t_player *player)
{
	static t_dot	lastpos;
	t_dot			newpos;

	SDL_GetMouseState(&newpos.x, &newpos.y);
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
	lastpos = (t_dot){newpos.x, newpos.y};
}

static void	keyboard_move(t_map *map, t_player *player, const Uint8 *state)
{
	map = NULL;
	if (state[SDL_SCANCODE_W])
	{
		player->vel.x += cos(player->dir) * player->const_vel;
		player->vel.y += -sin(player->dir) * player->const_vel;
	}
	if (state[SDL_SCANCODE_S])
	{
		player->vel.x += cos(player->dir + M_PI) * player->const_vel;
		player->vel.y += -sin(player->dir + M_PI) * player->const_vel;
	}
	if (state[SDL_SCANCODE_A])
	{
		player->vel.x += cos(player->dir + M_PI / 2) * player->const_vel;
		player->vel.y += -sin(player->dir + M_PI / 2) * player->const_vel;
	}
	if (state[SDL_SCANCODE_D])
	{
		player->vel.x += cos(player->dir - M_PI / 2) * player->const_vel;
		player->vel.y += -sin(player->dir - M_PI / 2) * player->const_vel;
	}
	/*if (state[SDL_SCANCODE_W])
		player->vel.y -= player->const_vel;
	if (state[SDL_SCANCODE_S])
		player->vel.y += player->const_vel;
	if (state[SDL_SCANCODE_A])
		player->vel.x -= player->const_vel;
	if (state[SDL_SCANCODE_D])
		player->vel.x += player->const_vel;*/
}

int			keyboard_state(t_map *map, t_player *player)
{
	const Uint8	*state;

	//printf(".");
	state = SDL_GetKeyboardState(NULL);
	player->vel = (t_fvector){};
	keyboard_move(map, player, state);
	mouse_move(player);
	return (0);
}
