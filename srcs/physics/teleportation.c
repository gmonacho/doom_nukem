#include "doom_nukem.h"

/*
**	Le d1p1 doit etre "relier" au d2p2
**	Le d1p2 doit etre "relier" au d2p1
*/

t_sector		*get_sector(t_map *map, t_linedef *line)
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
}

int				teleportation(t_map *tmap, t_sector *sector2,\
						t_linedef *line1, t_linedef *line2)
{
	t_sector	*sector1;

	//printf("Changes sector\n");
	sector1 = get_sector(tmap, line1);
	//printf("Sector 1 : %p\n", sector1);
	//printf("Sector 2 : %p\n", sector2);
	if (ft_abs(sector1->floor_height - sector2->floor_height) <\
														tmap->player.hitbox / 2)
	{
		tmap->player.pos = (t_fdot){	map(tmap->player.pos.x,\
									(t_dot){line1->p1.x, line1->p2.x},\
									(t_dot){line2->p2.x, line2->p1.x}),\
									map(tmap->player.pos.y,\
									(t_dot){line1->p1.y, line1->p2.y},\
									(t_dot){line2->p2.y, line2->p1.y})};
		tmap->player.dir -= line2->angle - line1->angle;
		tmap->player.pos = (t_fdot){tmap->player.pos.x +\
							cos(tmap->player.dir) * 2 * tmap->player.hitbox,\
									tmap->player.pos.y +\
							-sin(tmap->player.dir) * 2 * tmap->player.hitbox};
		tmap->player.sector = sector2;
		return (1);
	}
	//printf("Changes sector fin\n");
	return (0);
}
