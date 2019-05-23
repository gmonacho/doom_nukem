#include "doom_nukem.h"

/*
**	Le d1p1 doit etre "relier" au d2p2
**	Le d1p2 doit etre "relier" au d2p1
*/

int				teleportation(t_map *tmap, t_sector *sector2,\
						t_linedef *line1, t_linedef *line2)
{
	t_sector	*sector1;
	t_linedef	*tmp;

	//printf("Changes sector\n");
	sector1 = tmap->sectors;
	while (sector1)
	{
		tmp = sector1->lines;
		while (tmp)
		{
			if (line1 == tmp)
				break ;
			tmp = tmp->next;
		}
		sector1 = sector1->next;
	}
	if (ft_abs(sector1->floor_height - sector2->floor_height) <\
														tmap->player.hitbox / 2)
	{
		tmap->player.pos = (t_fdot){	map(tmap->player.pos.x,\
									(t_dot){line1->p1.x, line1->p2.x},\
									(t_dot){line2->p2.x, line2->p1.x}) +\
					(tmap->player.vel.x > 0 ? 1 : -1) * tmap->player.hitbox,\
									map(tmap->player.pos.y,\
									(t_dot){line1->p1.y, line1->p2.y},\
									(t_dot){line2->p2.y, line2->p1.y}) +\
					(tmap->player.vel.y > 0 ? 1 : -1) * tmap->player.hitbox};
		tmap->player.dir += line2->angle - line1->angle;
		return (1);
	}
	//printf("Changes sector fin\n");
	return (0);
}
