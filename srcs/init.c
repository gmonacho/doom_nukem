#include "doom_nukem.h"

/*
**	Produit vectoriel des vecteurs unitaire i et j
**	pour trouver le vecteur normal au plan
*/

int			init_polygone(t_poly *poly)
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
		poly->ii = poly->i.x * poly->i.x + poly->i.y * poly->i.y + poly->i.z * poly->i.z;
		poly->jj = poly->j.x * poly->j.x + poly->j.y * poly->j.y + poly->j.z * poly->j.z;
		poly->ij = poly->i.x * poly->j.x + poly->i.y * poly->j.y + poly->i.z * poly->j.z;
		poly->ijij_iijj = poly->ij * poly->ij - poly->ii * poly->jj;
		poly->equation = (t_plan){(t_fdot_3d){	(poly->i.y * poly->j.z - poly->i.z * poly->j.y) / 10000,\
												(poly->i.z * poly->j.x - poly->i.x * poly->j.z) / 10000,\
												(poly->i.x * poly->j.y - poly->i.y * poly->j.x) / 10000},\
									0};
		poly->equation.d = -(poly->equation.v.x * poly->dots[0].x + poly->equation.v.y * poly->dots[0].y + poly->equation.v.z * poly->dots[0].z);
		poly->equation_rotz_only = poly->equation;
		// if (!(poly->dots_proj = (t_dot *)malloc(sizeof(t_dot) * (N_DOTS_POLY + 1))))
		// 	return (ft_putendl_ret("Malloc dots_proj error\n", 1));
		// poly->dots_proj[N_DOTS_POLY] = NULL;
		poly = poly->next;
	}
	return (0);
}

void		init_player(t_win *win, t_player *player)
{
	// win->view = TEXTURE_VIEW;
	win->view = TEXTURE_VIEW | WALL_VIEW | BOX_VIEW;
	// player->dir_init = 0;

	translate_all(win->map->polys, (t_fdot_3d){-player->pos_up.x, -player->pos_up.y, -player->pos_up.z});
	translate_all_rotz_only(win->map->polys, (t_fdot_3d){-player->pos_up.x, -player->pos_up.y, -player->pos_up.z});
	rotate_all_rotz_only(win->map->polys, create_rz_matrix(-player->dir_init));

	player->inventory = define_inventory();
	player->fov = M_PI_4;
	player->fov_2 = player->fov / 2;
	player->fov_up = M_PI_4;
	player->fov_up_2 = player->fov_up / 2;
	player->rot_y = 0;
	player->ddir = 0.05;
	player->win_w = win->w;
	player->win_h = win->h;
	win->w_div_fov = win->w / player->fov;
	win->h_div_fov = win->h / player->fov_up;
	if (init_rays(win, player))
		return (ft_putendl("Erreur malloc rays"));
	player->maxHp = 50;
	player->currentHp = player->maxHp;
	player->maxArmor = 50;
	player->currentArmor = player->maxArmor;
	player->inventory->ammo = 15;
	player->inventory->magazine = 120;
	player->width_2 = player->width / 2;
	player->width_10 = player->width / 10;
	win->map->player.collision_on = 1;
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
<<<<<<< HEAD
=======
	
	t_fdot_3d	d1 = (t_fdot_3d){3, -5, 9};
	t_fdot_3d	d2 = (t_fdot_3d){-1, 1, -9};
	t_fdot_3d	ret;
>>>>>>> af4648869009f228ba11a4f0a2b0b7668c47a98c

	ret = return_rotate_dot(d1, create_rz_matrix(M_PI_2));
	printf("ret %f %f %f\n", ret.x, ret.y, ret.z);
	ret = return_rotate_dot(d2, create_rz_matrix(M_PI_2));
	printf("ret %f %f %f\n", ret.x, ret.y, ret.z);
	// exit(0);
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