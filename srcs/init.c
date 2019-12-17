#include "doom_nukem.h"

/*
**	Produit vectoriel des vecteurs unitaire i et j
**	pour trouver le vecteur normal au plan
*/

// int				print_polys(t_win *win, t_player *player)
// {
// 	t_poly		*poly;

// 	poly = win->map->polys;
// 	while (poly)
// 	{
// 		printf("Equation %f x + %f y + %f z + %f = 0\n", poly->equation.v.x, poly->equation.v.y, poly->equation.v.z, poly->equation.d);
// 		printf("D1 %f %f %f\n", poly->dots[0].x, poly->dots[0].y, poly->dots[0].z);
// 		printf("D2 %f %f %f\n", poly->dots[1].x, poly->dots[1].y, poly->dots[1].z);
// 		printf("D3 %f %f %f\n", poly->dots[2].x, poly->dots[2].y, poly->dots[2].z);
// 		printf("D4 %f %f %f\n", poly->dots[3].x, poly->dots[3].y, poly->dots[3].z);
// 		poly = poly->next;
// 	}
// }

int			init_polygone(t_poly *poly)
{
	while (poly)
	{
		// printf("Poly 3 pts : %f %f %f / %f %f %f / %f %f %f\n", poly->dots[0].x, poly->dots[0].y, poly->dots[0].z,\
		// 														poly->dots[1].x, poly->dots[1].y, poly->dots[1].z,\
		// 														poly->dots[N_DOTS_POLY - 1].x, poly->dots[N_DOTS_POLY - 1].y, poly->dots[N_DOTS_POLY - 1].z);
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
	// define_poly_shot(win, player);

	// t_plan		p = (t_plan){(t_fdot_3d){-1, 0, 0}, 250};
	// t_fdot_3d	translation = (t_fdot_3d){100, 100, 100};

	// p.d -= scalar_product(p.v, translation);
	// printf("Plan %f %f %f %f\tScalar product : %f\n", p.v.x, p.v.y, p.v.z, p.d, scalar_product(p.v, translation));
	// exit(0);
}

// int		init_textures(t_win *win, t_textures *textures)
// {
// 	if (!(textures->wall_1 = IMG_Load("textures/elephantride.png")) ||\
// 		!(textures->wall_2 = IMG_Load("textures/randomPNG/Brick.png")) ||\
// 		!(textures->floor = IMG_Load("textures/sol.png")) ||\
// 		!(textures->ceil = IMG_Load("textures/mur_pierre.png")) ||\
// 		!(win->rend_texture = SDL_CreateTexture(win->rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT)))
// 	{
// 		ft_putendl(SDL_GetError());
// 		return (1);
// 	}
// 	return (0);
// }


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