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
**	Il faut relancer le keyboard state pour avoir la vrai vel
*/

int				teleportation(t_win *win, t_map *map,\
								t_linedef *line1, t_linedef *line2)
{
	printf("Changes sector\n");
	if (line1->sector->floor_height + map->player.height / 2 >=\
										line2->sector->floor_height &&\
		map->player.height <= line2->sector->height)
	{
		if (line1->p2.x == line1->p1.x || line2->p2.x == line2->p1.x)
			map->player.dir -= line2->angle - line1->angle +\
			(sign(line1->p2.y - line1->p1.y) == sign(line2->p2.y - line2->p1.y) ?\
			M_PI : 0);
		else
			map->player.dir -= line2->angle - line1->angle +\
			(sign(line1->p2.x - line1->p1.x) == sign(line2->p2.x - line2->p1.x) ?\
			M_PI : 0);

		keyboard_state(win, &(map->player));

		map->player.pos = (t_fdot){prop(map->player.pos.x,\
									(t_dot){line1->p1.x, line1->p2.x},\
									(t_dot){line2->p2.x, line2->p1.x}),\
									prop(map->player.pos.y,\
									(t_dot){line1->p1.y, line1->p2.y},\
									(t_dot){line2->p2.y, line2->p1.y})\
									};

		map->player.pos = (t_fdot){map->player.pos.x +\
									(5 * map->player.vel.x) +\
									sign(map->player.vel.x) * map->player.width / 2,
									map->player.pos.y +\
									(5 * map->player.vel.y) +\
									sign(map->player.vel.y) * map->player.width / 2\
									};
		map->player.sector = line2->sector;
		return (1);
	}
	//printf("Changes sector fin\n");
	return (0);
}
