#include "doom_nukem.h"

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
			if (line->p1.x == line->p2.x && line->p1.y == line->p2.y)
			{
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

int		init_lines(t_map *map)
{
	t_sector	*sector;
	t_linedef	*line;

	sector = map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			line->destline = NULL;
			line->sector = sector;
			line->origin = (t_fdot_3d){line->p1.x, line->p1.y, sector->ceil_height};
			line->i = (t_fdot_3d){line->p2.x - line->p1.x, line->p2.y - line->p1.y, 0};
			line->j = (t_fdot_3d){0, 0, -sector->height};
			line->side = SIDE_RIGHT;
			if (line->flags & PORTAL && !find_portal_id(map, line, line->id))
				return (1);
			if (line->flags & PORTAL)
			{
				if (!(line->texture = IMG_Load("textures/sol.png")))
				{
					ft_putendl(SDL_GetError());
					return (1);
				}
			}
			else
				line->texture = map->textures.tortue;
			printf("Line : %p %p\n", line, line->destline);
			line = line->next;
			// if (set_clockwise(map, sector, line))
			// 	return ();
		}
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
		if (!(sector->ceil_texture = IMG_Load("textures/mur_pierre.png")) ||
			!(sector->floor_texture = IMG_Load("textures/sol.png")))
		{
			ft_putendl(SDL_GetError());
			return (1);
		}
		if (sector->floor_height >= sector->ceil_height)
			return (1);
		sector->height = sector->ceil_height - sector->floor_height;
		// printf("Ceil height = %d\n", sector->ceil_height);
		sector->ceil_equation =		(t_plan){(t_fdot_3d){0, 0, 1}, -sector->ceil_height};
		sector->floor_equation =	(t_plan){(t_fdot_3d){0, 0, 1}, -sector->floor_height};
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
	player->pos_up = (t_fdot_3d){0, 0, 0};
	// player->pos_up = (t_fdot_3d){	player->pos.x,\
	// 								player->pos.y,\
	// 								player->sector->floor_height + player->height};
	player->inventory = define_inventory();
	player->dir = 0;
	player->fov = _PI_4;
	player->dir_up = 0;
	player->fov_up = _PI_4;
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


	
	// t_fdot_3d		p1 = (t_fdot_3d){1, 1, 1};
	// t_fdot_3d		p2 = (t_fdot_3d){5, 5, 5};
	// t_fdot_3d		v = (t_fdot_3d){4, 4, 4};
	// for (int i = 0; i < 50; i++)
	// {
	// 	rotate_dot(&p1, player->rz);
	// 	rotate_dot(&p2, player->rz);
	// 	rotate_dot(&v, player->rz);

	// 	rotate_dot(&p1, player->ry);
	// 	rotate_dot(&p2, player->ry);
	// 	rotate_dot(&v, player->ry);
	// }
	// printf("p1 %f %f %f\n", p1.x, p1.y, p1.z);
	// printf("p2 %f %f %f\n", p2.x, p2.y, p2.z);
	// printf("Diff %f %f %f\n", p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
	// printf("Vector %f %f %f\n\n", v.x, v.y, v.z);
	// exit(0);
}

int		init_textures(t_textures *textures)
{
	if (!(textures->elephantride = IMG_Load("textures/walls/elephantride.png")) ||
		!(textures->tortue = IMG_Load("textures/walls/randomPNG/Brick.png")))
		// !(textures->tortue = IMG_Load("textures/walls/moine.png")))
		// !(textures->tortue = IMG_Load("textures/walls/tortue.png")))
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