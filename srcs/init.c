#include "doom_nukem.h"

/*
**	Produit vectoriel des vecteurs unitaire i et j
**	pour trouver le vecteur normal au plan
*/

void		init_polygone(t_poly *poly)
{
	int		i;

	i = 0;
	while (poly)
	{
		// printf("Poly 3 pts : %f %f %f / %f %f %f / %f %f %f\n", poly->dots[0].x, poly->dots[0].y, poly->dots[0].z,\
		// 														poly->dots[1].x, poly->dots[1].y, poly->dots[1].z,\
		// 														poly->dots[N_DOTS_POLY - 1].x, poly->dots[N_DOTS_POLY - 1].y, poly->dots[N_DOTS_POLY - 1].z);
		poly->index = i++;
		ft_memcpy(poly->dots_rotz_only, poly->dots, sizeof(t_fdot_3d) * 4);
		poly->dist12 = fdist_3d(poly->dots[0], poly->dots[1]);
		poly->dist14 = fdist_3d(poly->dots[0], poly->dots[N_DOTS_POLY - 1]);
		poly->i = (t_fdot_3d){	poly->dots[1].x - poly->dots[0].x,\
								poly->dots[1].y - poly->dots[0].y,\
								poly->dots[1].z - poly->dots[0].z};
		poly->j = (t_fdot_3d){	poly->dots[N_DOTS_POLY - 1].x - poly->dots[0].x,\
								poly->dots[N_DOTS_POLY - 1].y - poly->dots[0].y,\
								poly->dots[N_DOTS_POLY - 1].z - poly->dots[0].z};
		poly->i_rotz_only = poly->i;
		poly->j_rotz_only = poly->j;
		poly->ii = poly->i.x * poly->i.x + poly->i.y * poly->i.y + poly->i.z * poly->i.z;
		poly->jj = poly->j.x * poly->j.x + poly->j.y * poly->j.y + poly->j.z * poly->j.z;
		// poly->ij = poly->i.x * poly->j.x + poly->i.y * poly->j.y + poly->i.z * poly->j.z;
		// poly->ijij_iijj = poly->ij * poly->ij - poly->ii * poly->jj;
		poly->equation.v = (t_fdot_3d){(poly->i.y * poly->j.z - poly->i.z * poly->j.y) / 10000,\
										(poly->i.z * poly->j.x - poly->i.x * poly->j.z) / 10000,\
										(poly->i.x * poly->j.y - poly->i.y * poly->j.x) / 10000};
		poly->equation.d = -(poly->equation.v.x * poly->dots[0].x + poly->equation.v.y * poly->dots[0].y + poly->equation.v.z * poly->dots[0].z);
		poly->equation_rotz_only = poly->equation;
		poly = poly->next;
	}
	// printf("i = %d\n", i);
	// exit(0);
}

static void		init_player_maths(t_win *win, t_player *player)
{
	// win->view = TEXTURE_VIEW | WALL_VIEW | BOX_VIEW;
	win->view = TEXTURE_VIEW;
	translate_all(win->map->polys, (t_fdot_3d){-player->pos_up.x, -player->pos_up.y, -player->pos_up.z});
	translate_all_rotz_only(win->map->polys, (t_fdot_3d){-player->pos_up.x, -player->pos_up.y, -player->pos_up.z});
	rotate_all_rotz_only(win->map->polys, create_rz_matrix(-player->dir_init));
	player->rot_y = 0;
	player->ddir = 0.05;
	player->fov = win->w * M_PI_2 / 1000;
	player->fov_up = win->h * M_PI_2 / 1000;
	player->fov_2 = player->fov / 2;
	player->fov_up_2 = player->fov_up / 2;
	player->width_2 = player->width / 2;
	player->height_10 = player->height / 10;
	player->_9_height_10 = 9 * player->height_10;
	player->_4_height_10 = 4 * player->height_10;
	player->collision_on = 1;
	win->w_div_fov = win->w / player->fov;
	win->h_div_fov = win->h / player->fov_up;
	if (init_rays(win, player))
		return (ft_putendl("Erreur malloc rays"));
}

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
	start_cooldown(&(player->timers.item_cd), 200);
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

int			init_win_player(t_win *win, t_player *player)
{
	if (!(win->pixels = (Uint32 *)malloc(sizeof(Uint32) * win->w * win->h)))
		return (1);
	init_player_maths(win, player);
	init_player_hud(player);
	// exit(0);
	return (0);
}

int			init_music(t_doom_music	*music)
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
	return (1);
}