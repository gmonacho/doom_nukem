#include "doom_nukem.h"

/*
**	Le floor et ceil ont une forme quelconque donc
**	le plan est infini -> Origin : 0,0,h
*/

void	init_polygone(t_poly *poly)		//ATTENTION NE MARCHE PAS POUR LES PLANS INCLINE !!!
{
	double	alpha;

	while (poly)
	{
		poly->dist12 = fdist_3d(poly->d1, poly->d2);
		poly->dist13 = fdist_3d(poly->d1, poly->d3);
		poly->i = (t_fdot_3d){	poly->d2.x - poly->d1.x,\
								poly->d2.y - poly->d1.y,\
								poly->d2.z - poly->d1.z};
		poly->j = (t_fdot_3d){	poly->d3.x - poly->d1.x,\
								poly->d3.y - poly->d1.y,\
								poly->d3.z - poly->d1.z};
		alpha = poly->d2.x - poly->d1.x ? atan((poly->d2.y - poly->d1.y) /\
										(double)(poly->d2.x - poly->d1.x)) :\
										M_PI_2;
		if (alpha < 0)
			alpha += M_PI;
		poly->equation = (t_plan){(t_fdot_3d){cos(alpha - M_PI_2),\
											sin(alpha - M_PI_2),\
											0},\
									0};
		poly->equation.d = -(poly->equation.v.x * poly->d1.x + poly->equation.v.y * poly->d1.y);
		poly = poly->next;
	}
}

void	init_player(t_win *win, t_player *player)
{
	player->win_w = win->w;
	player->win_h = win->h;
	printf("Pos player 2d %f %f\n", player->pos.x, player->pos.y);
	player->pos_up = (t_fdot_3d){0, 0, 0};
	translate_all(win->map->polys, (t_fdot_3d){-player->pos.x, -player->pos.y, -player->sector->floor_height - player->height});

	t_poly	*poly = win->map->polys;
	while (poly)
	{
		printf("Equation %f x + %f y + %f z + %f = 0\n", poly->equation.v.x, poly->equation.v.y, poly->equation.v.z, poly->equation.d);
		poly = poly->next;
	}

	player->inventory = define_inventory();
	player->fov = _PI_4;
	player->fov_2 = _PI_4 / 2;
	player->fov_up = _PI_4;
	player->fov_up_2 = _PI_4 / 2;
	// player->dir = 0;
	// player->dir_up = 0;
	player->ddir = 0.1;
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

int		init_textures(t_textures *textures)
{
	if (!(textures->wall_1 = IMG_Load("textures/walls/elephantride.png")) ||\
		!(textures->wall_2 = IMG_Load("textures/walls/randomPNG/Brick.png")) ||\
		!(textures->floor = IMG_Load("textures/sol.png")) ||\
		!(textures->ceil = IMG_Load("textures/mur_pierre.png")))
	{
		ft_putendl(SDL_GetError());
		return (1);
	}
	return (0);
}


int		init_music(t_doom_music	*music)
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