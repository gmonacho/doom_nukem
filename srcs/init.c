#include "doom_nukem.h"

static void		init_player_hud(t_player *player)
{
	player->inventory = define_inventory();
	player->maxHp = 100;
	player->currentHp = player->maxHp;
	player->maxArmor = 100;
	player->currentArmor = player->maxArmor;
	player->inventory->ammo = 15;
	player->inventory->magazine = 120;
	start_cooldown(&(player->timers.bullet_cd), 130);
	start_cooldown(&(player->timers.item_cd), 100);
	start_cooldown(&(player->timers.text_cd), 600);
	start_cooldown(&(player->timers.reload_cd), 600);
	start_cooldown(&(player->timers.animation_cd), 1000);
	start_cooldown(&(player->timers.shot_cd), 50);
	start_cooldown(&(player->timers.animation_shot_cd), 10);
	start_cooldown(&(player->timers.mouse), 10);
	player->timers.reload_cd.index = 5;
	player->timers.bullet_cd.index = 5;
	player->timers.bullet_cd.index = 0;
}

int				init_win_player(t_win *win, t_player *player)
{
	if (!(win->pixels = (Uint32 *)malloc(sizeof(Uint32) * win->w * win->h)))
		return (1);
	start_cooldown(&(win->view_change_time), 250);
	start_cooldown(&(win->map->objects_animation), 20);
	start_cooldown(&(win->map->objects_tp_timer), 2000);
	start_cooldown(&(player->interaction_inventaire_time), 250);
	start_cooldown(&(player->fly_timer), 250);
	init_player_maths(win, player);
	init_player_hud(player);
	return (0);
}

int				init_music_timer(t_map *map, t_doom_music *music)
{
	if (music)
	{
		if (!(music->editor_music = Mix_LoadMUS("sounds/map_editor.wav")))
			return (ret_error(SDL_GetError()));
		if (!(music->credit_music = Mix_LoadMUS("sounds/Remember_chill.mp3")))
			return (ret_error(SDL_GetError()));
		if (!(music->menu_music = Mix_LoadMUS("sounds/doomMenu.wav")))
			return (ret_error(SDL_GetError()));
	}
	map = NULL;
	return (1);
}
