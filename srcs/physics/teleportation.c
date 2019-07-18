#include "doom_nukem.h"

/*
**	Le d1p1 doit etre "relier" au d2p2
**	Le d1p2 doit etre "relier" au d2p1
**	Selon le sens de la demi-droite [p1p2) j'ajoute PI ou pas a son angle
**	car le coef directeur est le meme pour les 2
**
**	Il faut relancer le keyboard state pour avoir la vrai vel
*/

int				teleportation(t_win *win, t_map *tmap,\
								t_linedef *line1, t_linedef *line2)
{
	//printf("Changes sector\n");
	if (line1->sector->floor_height + tmap->player.height / 2 >=\
										line2->sector->floor_height &&\
		tmap->player.height <= line2->sector->height)
	{
		tmap->player.dir -= line2->angle - line1->angle +\
		(sign(line1->p2.x - line1->p1.x) == sign(line2->p2.x - line2->p1.x) ?\
		M_PI : 0);

		keyboard_state(win, &(tmap->player));

		tmap->player.pos = (t_fdot){map(tmap->player.pos.x,\
									(t_dot){line1->p1.x, line1->p2.x},\
									(t_dot){line2->p2.x, line2->p1.x}),\
									map(tmap->player.pos.y,\
									(t_dot){line1->p1.y, line1->p2.y},\
									(t_dot){line2->p2.y, line2->p1.y})};

		tmap->player.pos = (t_fdot){tmap->player.pos.x +\
									(10 * tmap->player.vel.x),\
									tmap->player.pos.y +\
									(10 * tmap->player.vel.y)};
		tmap->player.sector = line2->sector;
		return (1);
	}
	//printf("Changes sector fin\n");
	return (0);
}
