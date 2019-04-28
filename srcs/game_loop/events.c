#include "doom_nukem.h"

static void	keyboard_move(t_map *map, t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_W])
	{
		player->vel.x += cos(player->dir) * player->const_vel;
		player->vel.y += -sin(player->dir) * player->const_vel;
	}
	if (state[SDL_SCANCODE_S])
	{
		player->vel.x += cos(player->dir + PI) * player->const_vel;
		player->vel.y += -sin(player->dir + PI) * player->const_vel;
	}
	if (state[SDL_SCANCODE_A])
	{
		player->vel.x += cos(player->dir + PI / 2) * player->const_vel;
		player->vel.y += -sin(player->dir + PI / 2) * player->const_vel;
	}
	if (state[SDL_SCANCODE_D])
	{
		player->vel.x += cos(player->dir - PI / 2) * player->const_vel;
		player->vel.y += -sin(player->dir - PI / 2) * player->const_vel;
	}
}

int			keyboard_state(t_map *map, t_player *player)
{
	const Uint8	*state;

	state = SDL_GetKeyboardState(NULL);
	player->vel = (t_vector){.x = 0, .y = 0};
	keyboard_move(map, player, state);
}