#include "doom_nukem.h"

/*
**	Le d1p1 doit etre "relier" au d2p2
**	Le d1p2 doit etre "relier" au d2p1
*/

/*static t_sector	*get_sector(t_map *map, t_linedef *line)
{
	t_sector	*sector;
	t_linedef	*tmp;

	sector = map->sectors;
	while (sector)
	{
		tmp = sector->lines;
		while (tmp)
		{
			if (line == tmp)
				return (sector);
			tmp = tmp->next;
		}
		sector = sector->next;
	}
	return (NULL);
}*/

int				teleportation(t_map *tmap,\
								t_linedef *line1, t_linedef *line2)
{
	//t_sector	*sector1;

	//printf("Changes sector\n");
	//sector1 = get_sector(tmap, line1);
	if (line1->sector->floor_height + tmap->player.height / 2 >=\
										line2->sector->floor_height &&\
		tmap->player.height <= line2->sector->height)
	{
		tmap->player.pos = (t_fdot){map(tmap->player.pos.x,\
									(t_dot){line1->p1.x, line1->p2.x},\
									(t_dot){line2->p2.x, line2->p1.x}),\
									map(tmap->player.pos.y,\
									(t_dot){line1->p1.y, line1->p2.y},\
									(t_dot){line2->p2.y, line2->p1.y})};

		tmap->player.dir -= line2->angle - line1->angle +\
		(sign(line1->p2.x - line1->p1.x) == sign(line2->p2.x - line2->p1.x) ?\
		M_PI : 0);

		tmap->player.pos = (t_fdot){tmap->player.pos.x +\
		cos(tmap->player.dir) * (tmap->player.const_vel + tmap->player.width / 2),\
									tmap->player.pos.y +\
		-sin(tmap->player.dir) * (tmap->player.const_vel + tmap->player.width / 2)};

		tmap->player.sector = line2->sector;
		return (1);
	}
	//printf("Changes sector fin\n");
	return (0);
}
