#include "doom_nukem.h"

/*
**	Antoine t'es un encule le cercle trigo EST INVERSE oublie pas, wtf t debile
**
**	Ajouts :
**	- Limite la recherche de mur a la taille de la hitbox ! opti
**	- Gliss sur les murs
**
**	s = perimetre / 2 = somme des trois cotes / 2
**	On melange 2 formules sur l'aire du triangle : base * h / 2 = sqrt(s(s-a)(s-b)(s-c))
**	Et boom on a h par rapport aux 3 cotes.
**
**
**	Nouveau systeme collisions :
**	-	Raycasting dans la direction physique du player
**	-	de la largeur de la hitbox si possible
**	-	Il faut inverser le cercle trigo pour que l'angle soit coherent
**			avec le repere.
*/

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

//Enum pour vertical, horizontal, quelquonque ????

// static void		teleportation(t_win *win, t_map *map,\
// 								t_linedef *line1, t_linedef *line2)
// {
// 	double		p;
//     t_music		*music;

//     music = NULL;
// 	p = 0;
// 	set_ray_angle(&(map->player.dir), line1, line2);

// 	keyboard_state(win, &(map->player), music);	//Reupere la nouvelle direction grace au nouvel angle

// 	set_new_position(&(map->player.pos), line1, line2, &(map->player.sector));
// 	// set_new_position_3d(&(map->player.pos_up), line1, line2, &(map->player.sector));
// 	map->player.pos.x += (map->player.vel.x / map->player.const_vel) * map->player.width_2;
// 	map->player.pos.y += (map->player.vel.y / map->player.const_vel) * map->player.width_2;

// 	if (!(line1->flags & PORTAL) ||\
// 	((line1->flags & PORTAL) &&\
// 	(line2->sector->floor_height - line1->sector->floor_height >\
// 												map->player.height / 2 ||\
// 	line2->sector->ceil_height - line1->sector->floor_height <\
// 												map->player.height ||\
// 	line1->sector->ceil_height - line2->sector->floor_height <\
// 												map->player.height ||\
// 	line2->sector->height < map->player.height)))
// 	{
// 		map->player.vel = (t_fvector){0, 0};
// 		return (1);
// 	}
// 	else if (dist <= 3 * map->player.const_vel) //Ici distance tp collision entre portails
// 	{
// 		// printf("Tp !\n");
// 		teleportation(win, map, line1, line2);
// 	}
// 	return (0);
// }

// static int	collisions(t_win *win, t_map *map, t_player *player)
// {
// 	t_calculs	calculs;
// 	t_linedef	*collision;
// 	t_fdot		newpos;
// 	int			i;

// 	calculs.raycast = 0;
// 	// printf("Vel = %f\t%f\n", player->vel.x, player->vel.y);
// 	if (player->vel.x)
// 		calculs.ray.angle = atan(player->vel.y / player->vel.x);
// 	else
// 		calculs.ray.angle = M_PI_2;

// 	// printf("Angle 1 : %f pi\n", calculs.ray.angle / M_PI);
// 	if (player->vel.x < 0)
// 		calculs.ray.angle += M_PI;
// 	else if (calculs.ray.angle < 0)
// 		calculs.ray.angle += _2_PI;

// 	newpos = (t_fdot){player->pos.x + player->vel.x,\
// 						player->pos.y + player->vel.y};
// 	i = -1;
// 	while (++i < 8)	//lance 8 rayons autour de la nouvelle pos du player
// 	{
// 		set_ray_equation(win, player, &(calculs.ray), newpos);
// 		calculs.dist = 0;
// 		collision = intersection_ray_wall(win, player, &newpos, player->sector, &calculs);
// 		// if (!collision)
// 		// printf("Angle 2 : %f pi\ta = %f\tb = %f\t%f\n", calculs.ray.angle / M_PI, calculs.ray.a, calculs.ray.b, player->width_2);
// 		// 	printf("WTTFFF ??? WALL NULL collision : %p\n", collision);
// 		if (calculs.dist < player->width_2 && collision)
// 			return (actions(win, map, collision, calculs.dist));
// 		calculs.ray.angle += _PI_4;
// 		if (calculs.ray.angle >= _2_PI)
// 			calculs.ray.angle -= _2_PI;
// 	}
// 	return (0);
// }

// int		physics(t_win *win, t_map *map, t_player *player)
// {
// 	if (player->z > 40)
// 		player->jump = 0;
// 	if (player->z > 5)
// 		player->z -= 5;
// 	else
// 		player->jump = 1;
// 	if (!player->vel.x && !player->vel.y)
// 		return (0);
	
// 	/*
// 	** --------------------------------------------------------------------------
// 	*/
// 	collisions(win, map, player);
// 	set_origin_rays(player->rays, player->pos_up);
// 	// player->pos = (t_fdot_3d){	player->pos.x + player->vel.x,\
// 	// 							player->pos.y + player->vel.y,\
// 	// 							player->pos.z};
// 	return (0);
// }
