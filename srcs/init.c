#include "doom_nukem.h"

/*
**	Le floor et ceil ont une forme quelconque donc
**	le plan est infini -> Origin : 0,0,h
*/

static int		find_portal_id(t_map *map, t_linedef *line1, int id)
{
	t_sector	*sector;
	t_linedef	*line;

	sector = map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			if (id == line->id && line1 != line)
			{
				line1->destline = line;
				return (1);
			}
			line = line->next;
		}
		sector = sector->next;
	}
	return (0);
}

// static int	set_clockwise(t_map *map, t_sector *sector, t_linedef *line)
// {
// 	if (line->next && line->next->side == line->side)
// 	{
// 		if (line->next->p1.x == line->p2.x && line->next->p1.y == line->p2.y)
// 			return (0);
// 		else
// 			ft_swap(&(line->next->p1), &(line->next->p2));
// 	}
// 	else
// 	{

// 	}
// }

static void	check_lines(t_map *map)
{
	t_sector	*sector;
	t_linedef	*line;
	t_linedef	*last;
	t_linedef	*tmp;

	sector = map->sectors;
	while (sector)
	{
		last = NULL;
		line = sector->lines;
		while (line)
		{
			if (line->flags & (PORTAL | WALL) && line->p1.x == line->p2.x && line->p1.y == line->p2.y)
			{
				printf("Destroy line : %d %d \t %d %d\n", line->p1.x, line->p1.y, line->p2.x, line->p2.y);
				if (last)
					last->next = line->next;
				else
					sector->lines = line->next;
				ft_strdel(&(line->name));
				tmp = line;
				line = line->next;
				free(tmp);
			}
			else
			{
				last = line;
				line = line->next;
			}
		}
		sector = sector->next;
	}
}

static int		add_floor(t_sector *sector, t_linedef **last)
{
	t_linedef	*new;

	if (!(new = (t_linedef *)malloc(sizeof(t_linedef))))
		return (1);
	new->next = NULL;
	new->equation = (t_plan){(t_fdot_3d){0, 0, 1}, -sector->floor_height};
	new->origin = (t_fdot_3d){0, 0, sector->floor_height};
	new->i = (t_fdot_3d){1, 0, 0};
	new->j = (t_fdot_3d){0, 1, 0};
	new->id = 0;
	new->flags = FLOOR;
	if (!(new->texture = IMG_Load("textures/sol.png")))
	{
		ft_putendl(SDL_GetError());
		return (1);
	}
	new->sector = sector;
	new->destline = NULL;
	(*last)->next = new;
	*last = new;
	return (0);
}

static int		add_ceil(t_sector *sector, t_linedef *last)
{
	t_linedef	*new;

	if (!(new = (t_linedef *)malloc(sizeof(t_linedef))))
		return (1);
	new->next = NULL;
	new->equation = (t_plan){(t_fdot_3d){0, 0, 1}, -sector->ceil_height};
	new->origin = (t_fdot_3d){0, 0, sector->ceil_height};
	new->i = (t_fdot_3d){1, 0, 0};
	new->j = (t_fdot_3d){0, 1, 0};
	new->id = 0;
	new->flags = CEIL;
	new->sector = sector;
	new->destline = NULL;
	if (!(new->texture = IMG_Load("textures/mur_pierre.png")))
	{
		ft_putendl(SDL_GetError());
		return (1);
	}
	last->next = new;
	return (0);
}

int		init_lines(t_map *map)
{
	t_sector	*sector;
	t_linedef	*last_line;
	t_linedef	*line;

	sector = map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			line->sector = sector;
			line->destline = NULL;
			init_equation(line);
			line->origin = (t_fdot_3d){line->p1.x, line->p1.y, sector->ceil_height};
			line->i = (t_fdot_3d){line->p2.x - line->p1.x, line->p2.y - line->p1.y, 0};
			line->j = (t_fdot_3d){0, 0, -sector->height};

			line->poly_3d[0] = (t_fdot_3d){line->p1.x, line->p1.y, sector->ceil_height};
			line->poly_3d[1] = (t_fdot_3d){line->p2.x, line->p2.y, sector->ceil_height};
			line->poly_3d[2] = (t_fdot_3d){line->p1.x, line->p1.y, sector->floor_height};
			line->poly_3d[3] = (t_fdot_3d){line->p2.x, line->p2.y, sector->floor_height};

			line->side = SIDE_RIGHT;
			if (line->flags & PORTAL && !find_portal_id(map, line, line->id))
				return (1);
			line->texture = line->flags & PORTAL ? map->textures.floor :\
													map->textures.wall_2;
			printf("Line : %p %p\n", line, line->destline);
			last_line = line;
			line = line->next;
			// if (set_clockwise(map, sector, line))
			// 	return ();
		}
		if (add_floor(sector, &last_line) ||\
			add_ceil(sector, last_line))
			return (1);
		sector = sector->next;
	}
	check_lines(map);
	return (0);
}

// static void		set_center(t_map *map)
// {
// 	t_sector	*sector;
// 	t_linedef	*line;
// 	int			ndot;

// 	ndot = 0;
// 	sector = map->sectors;
// 	while (sector)
// 	{
// 		ndot = 0;
// 		line = sector->lines;
// 		while (line)
// 		{
// 			sector->center.x += line->p1.x;
// 			sector->center.y += line->p1.y;
// 			ndot++;
// 			line = line->next;
// 		}
// 		if (ndot)
// 		{
// 			sector->center.x /= ndot;
// 			sector->center.y /= ndot;
// 		}
// 		printf("Center : %d\t%d\tndot : %d\n", sector->center.x, sector->center.y, ndot);
// 		sector = sector->next;
// 	}
// }

int		init_sectors(t_map *map, t_player *player)
{
	t_sector	*sector;
	int			i;

	//set_center(map);
	sector = map->sectors;
	while (sector)
	{
		if (sector->floor_height >= sector->ceil_height)
			return (1);
		sector->height = sector->ceil_height - sector->floor_height;
		// printf("Ceil height = %d\n", sector->ceil_height);
		// sector->ceil_equation =		(t_plan){(t_fdot_3d){0, 0, 1}, -sector->ceil_height};
		// sector->floor_equation =	(t_plan){(t_fdot_3d){0, 0, 1}, -sector->floor_height};
		sector = sector->next;
	}
	player->sector = map->sectors;
	i = -1;
	while (++i < player->numsector)
		player->sector = player->sector->next;
	return (0);
}

void	init_player(t_win *win, t_player *player)
{
	player->win_w = win->w;
	player->win_h = win->h;
	printf("Pos player 2d %f %f\n", player->pos.x, player->pos.y);
	player->pos_up = (t_fdot_3d){0, 0, 0};
	translate_all(win->map->sectors, (t_fdot_3d){-player->pos.x, -player->pos.y, -player->sector->floor_height - player->height});

	t_sector *sector;
	t_linedef	*line;
	sector = win->map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			printf("Equation %f x + %f y + %f z + %f = 0\n", line->equation.v.x, line->equation.v.y, line->equation.v.z, line->equation.d);
			line = line->next;
		}
		sector = sector->next;
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
	define_line_shot(win, player);


	printf("Modulo %f\n", modulo(-46, 32));
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