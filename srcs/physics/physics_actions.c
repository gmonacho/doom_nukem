#include "doom_nukem.h"

/*
**	Le d1p1 doit etre "relier" au d2p2
**	Le d1p2 doit etre "relier" au d2p1
**	
**	Selon le sens de la demi-droite [p1p2) j'ajoute PI ou pas a son angle
**	car le coef directeur est le meme pour les 2 directions :
**	Si les deux points p1 sont avant les deux points p2 alors
**	il faut faire une rotation de 180 degres
**
**	Attention cercle trigo inverse
**
**	Il faut relancer le keyboard state pour avoir la vrai vel avec la nouvelle dir
**	On multiplie la vel pour avoir une distance de hitbox / 2
*/

//Enum pour vertical, horizontal, quelquonque

void			set_new_position(t_fdot *pos, t_linedef *line1, t_linedef *line2, t_sector **sector)
{
	double		p;

	// printf("New position\n");
	if (line1->p2.x == line1->p1.x)
		p = prop(pos->y,\
				(t_dot){line1->p1.y, line1->p2.y},\
				(t_dot){0, 10000});
	else
		p = prop(pos->x,\
				(t_dot){line1->p1.x, line1->p2.x},\
				(t_dot){0, 10000});
	// printf("Prop : %f\tposy = %f\tx = %d\ty = %d\n", p, pos->y, line1->p1.y, line1->p2.y);
	pos->x = prop(p,	(t_dot){0, 10000},\
						(t_dot){line2->p2.x, line2->p1.x});
	pos->y = prop(p,	(t_dot){0, 10000},\
						(t_dot){line2->p2.y, line2->p1.y});
	*sector = line2->sector;
}

static void				teleportation(t_win *win, t_map *map,\
								t_linedef *line1, t_linedef *line2)
{
	double				p;

	// printf("Teleportation\n");
	p = 0;
	// if (line1->p2.x == line1->p1.x)
	// {
	// 	printf("- 1 \n");
	// 	p = prop(map->player.pos.y,\
	// 			(t_dot){line1->p1.y, line1->p2.y},\
	// 			(t_dot){0, 1000});
	// 	if (line2->p2.x == line2->p1.x)
	// 	{
	// 		printf("-- 1 \n");
	// 		if (sign(line1->p2.y - line1->p1.y) == sign(line2->p2.y - line2->p1.y))
	// 			map->player.dir -= line2->angle - line1->angle +\
	// 								(line1->side == line2->side ? M_PI : 0);
	// 		else
	// 			map->player.dir -= line2->angle - line1->angle +\
	// 								(line1->side == line2->side ? 0 : M_PI);
	// 		if (sign(line1->p2.y - line1->p1.y) == sign(line2->p2.y - line2->p1.y))
	// 			printf("--- 1");
	// 		else
	// 			printf("--- 2");
	// 	}
	// 	else
	// 	{
	// 		printf("-- 2 \n");
	// 		if (sign(line1->p2.y - line1->p1.y) == sign(line2->p2.x - line2->p1.x))
	// 			map->player.dir -= line2->angle - line1->angle +\
	// 								(line1->side == line2->side ? M_PI : 0);
	// 		else
	// 			map->player.dir -= line2->angle - line1->angle +\
	// 								(line1->side == line2->side ? 0 : M_PI);
	// 		if (sign(line1->p2.y - line1->p1.y) == sign(line2->p2.x - line2->p1.x))
	// 			printf("--- 1");
	// 		else
	// 			printf("--- 2");
	// 	}
	// }
	// else
	// {
	// 	printf("- 2 \n");
	// 	p = prop(map->player.pos.x,\
	// 			(t_dot){line1->p1.x, line1->p2.x},\
	// 			(t_dot){0, 1000});
	// 	if (line2->p2.x == line2->p1.x)
	// 	{
	// 		printf("-- 1 \n");
	// 		if (sign(line1->p2.x - line1->p1.x) == sign(line2->p2.y - line2->p1.y))
	// 			map->player.dir -= line2->angle - line1->angle +\
	// 								(line1->side == line2->side ? M_PI : 0);
	// 		else
	// 			map->player.dir -= line2->angle - line1->angle +\
	// 								(line1->side == line2->side ? 0 : M_PI);
	// 		if (sign(line1->p2.x - line1->p1.x) == sign(line2->p2.y - line2->p1.y))
	// 			printf("--- 1");
	// 		else
	// 			printf("--- 2");
	// 	}
	// 	else
	// 	{
	// 		printf("-- 2 \n");
	// 		if (sign(line1->p2.x - line1->p1.x) == sign(line2->p2.x - line2->p1.x))
	// 			map->player.dir -= line2->angle - line1->angle +\
	// 								(line1->side == line2->side ? M_PI : 0);
	// 		else
	// 			map->player.dir -= line2->angle - line1->angle +\
	// 								(line1->side == line2->side ? 0 : M_PI);
	// 		if (sign(line1->p2.x - line1->p1.x) == sign(line2->p2.x - line2->p1.x))
	// 			printf("--- 1");
	// 		else
	// 			printf("--- 2");
	// 	}
	// }
	// keyboard_state(win, &(map->player));
	// map->player.pos = (t_fdot){	prop(p, (t_dot){0, 1000},\
	// 							(t_dot){line2->p2.x, line2->p1.x}) +\
	// 							map->player.vel.x * map->player.width_10 * 5,\
	// 							prop(p, (t_dot){0, 1000},\
	// 							(t_dot){line2->p2.y, line2->p1.y}) +\
	// 							map->player.vel.y * map->player.width_10 * 5\
	// 							};

	// if (line1->p2.x == line1->p1.x || line2->p2.x == line2->p1.x)
	// 	map->player.dir -= line2->angle - line1->angle +\
	// 	(sign(line1->p2.y - line1->p1.y) == sign(line2->p2.y - line2->p1.y) ?\
	// 	M_PI : 0);
	// else
	// 	map->player.dir -= line2->angle - line1->angle +\
	// 	(sign(line1->p2.x - line1->p1.x) == sign(line2->p2.x - line2->p1.x) ?\
	// 	M_PI : 0);

	// printf("Angle : %f pi\n", map->player.dir / M_PI);
	set_ray_angle(&(map->player.dir), line1, line2);
	// printf("Angle : %f pi\n\n", map->player.dir / M_PI);

	keyboard_state(win, &(map->player));	//Reupere la nouvelle direction grace au nouvel angle

	set_new_position(&(map->player.pos), line1, line2, &(map->player.sector));
	map->player.pos.x += (map->player.vel.x / map->player.const_vel) * map->player.width_2;
	map->player.pos.y += (map->player.vel.y / map->player.const_vel) * map->player.width_2;

	// if (wall->p1.x == wall->p2.x)
	// 	x_texture = (int)prop(calculs->closest.y,\
	// 				(t_dot){wall->p1.y, wall->p2.y},\
	// 				(t_dot){0, wall->texture->w - 1});
	// else
	// 	x_texture = (int)prop(calculs->closest.x,\
	// 				(t_dot){wall->p1.x, wall->p2.x},\
	// 				(t_dot){0, wall->texture->w - 1});

	// map->player.pos = (t_fdot){prop(map->player.pos.x,\
	// 							(t_dot){line1->p1.x, line1->p2.x},\
	// 							(t_dot){line2->p2.x, line2->p1.x}),\
	// 							prop(map->player.pos.y,\
	// 							(t_dot){line1->p1.y, line1->p2.y},\
	// 							(t_dot){line2->p2.y, line2->p1.y})\
	// 							};
	// map->player.pos = (t_fdot){map->player.pos.x +\
	// 							(5 * map->player.vel.x) +\
	// 							sign(map->player.vel.x) * map->player.width / 2,
	// 							map->player.pos.y +\
	// 							(5 * map->player.vel.y) +\
	// 							sign(map->player.vel.y) * map->player.width / 2\
	// 							};
}

int		actions(t_win *win, t_map *map, t_linedef *line1, double dist)
{
	t_linedef	*line2;

	line2 = line1->destline;
	// printf("Actions ! Portal -> 2 == %d\t%f\t%f\n", line1->flags & PORTAL, dist, map->player.const_vel);
	// if (line2)
	// {
	// 	printf("1 : %d\n", line1->sector->floor_height + map->player.height / 2 >=\
	// 									line2->sector->floor_height);
	// 	printf("2 : %d\n", line1->sector->floor_height + map->player.height <=\
	// 									line2->sector->ceil_height);
	// 	printf("3 : %d\n", line1->sector->floor_height + 3 * map->player.height / 2 <=\
	// 									line1->sector->ceil_height);
	// 	printf("4 : %d\n", map->player.height <= line2->sector->height);
	// 	printf("5 : %d\n", dist < map->player.const_vel);
	// }
	if (!(line1->flags & PORTAL) ||\
	((line1->flags & PORTAL) &&\
	(line2->sector->floor_height - line1->sector->floor_height >\
												map->player.height / 2 ||\
	line2->sector->ceil_height - line1->sector->floor_height <\
												map->player.height ||\
	line1->sector->ceil_height - line2->sector->floor_height <\
												map->player.height ||\
	line2->sector->height < map->player.height)))
	{
		//  printf("Saut trop grand ? Plafond 2/sol 1 ? Sol 2/plafond 1 trop petit ? Secteur 2 trop petit ? %d %d %d %d\n",\
		// 	line2->sector->floor_height - line1->sector->floor_height >\
		// 										map->player.height / 2,\
		// 	line2->sector->ceil_height - line1->sector->floor_height <\
		// 										map->player.height,\
		// 	line1->sector->ceil_height - line2->sector->floor_height <\
		// 										map->player.height,\
		// 	line2->sector->height < map->player.height);
		map->player.vel = (t_fvector){0, 0};
		return (1);
	}
	else if (dist <= 3 * map->player.const_vel) //Ici distance tp collision entre portails
	{
		// printf("Tp !\n");
		teleportation(win, map, line1, line2);
	}
	return (0);
}



// static int				teleportation(t_win *win, t_map *map,\
// 								t_linedef *line1, t_linedef *line2)
// {
// 	printf("Changes sector\n");
// 	if (line1->sector->floor_height + map->player.height / 2 >=\
// 										line2->sector->floor_height &&\
// 		line1->sector->floor_height + map->player.height <=\
// 										line2->sector->ceil_height &&\
// 		line1->sector->floor_height + 3 * map->player.height / 2 <=\
// 										line1->sector->ceil_height &&\
// 		map->player.height <= line2->sector->height)
// 	{
// 		if (line1->p2.x == line1->p1.x || line2->p2.x == line2->p1.x)
// 			map->player.dir -= line2->angle - line1->angle +\
// 			(sign(line1->p2.y - line1->p1.y) == sign(line2->p2.y - line2->p1.y) ?\
// 			M_PI : 0);
// 		else
// 			map->player.dir -= line2->angle - line1->angle +\
// 			(sign(line1->p2.x - line1->p1.x) == sign(line2->p2.x - line2->p1.x) ?\
// 			M_PI : 0);

// 		keyboard_state(win, &(map->player));

// 		map->player.pos = (t_fdot){prop(map->player.pos.x,\
// 									(t_dot){line1->p1.x, line1->p2.x},\
// 									(t_dot){line2->p2.x, line2->p1.x}),\
// 									prop(map->player.pos.y,\
// 									(t_dot){line1->p1.y, line1->p2.y},\
// 									(t_dot){line2->p2.y, line2->p1.y})\
// 									};

// 		map->player.pos = (t_fdot){map->player.pos.x +\
// 									(5 * map->player.vel.x) +\
// 									sign(map->player.vel.x) * map->player.width / 2,
// 									map->player.pos.y +\
// 									(5 * map->player.vel.y) +\
// 									sign(map->player.vel.y) * map->player.width / 2\
// 									};
// 		map->player.sector = line2->sector;
// 		return (1);
// 	}
// 	//printf("Changes sector fin\n");
// 	return (0);
// }

// int				actions(t_win *win, t_map *map, t_linedef *portal, t_player *player)
// {
// 	if (!(portal->flags & PORTAL) ||\
// 		(portal->flags & PORTAL &&\
// 		!teleportation(win, map, portal, portal->destline)))
// 	{
// 		player->vel = (t_fvector){0, 0};
// 		return (1);
// 	}
// 	return (0);
// }
