#include "doom_nukem.h"

void		ft_find_coord_p1(t_sector *sector, char *tab)
{
	sector->lines->p1.x = ft_atoi(ft_strrchr(tab, '(') + 1);
	sector->lines->p1.y = ft_atoi(ft_strrchr(tab, ',') + 1);
}

void		ft_find_coord_p2(t_sector *sector, char *tab)
{
	sector->lines->p2.x = ft_atoi(ft_strrchr(tab, '(') + 1);
	sector->lines->p2.y = ft_atoi(ft_strrchr(tab, ',') + 1);
}

int 	ft_chose_type(char *tab)
{	
	int type;

	type = 0;
	if (!ft_strcmp(tab, "WALL"))
		type = WALL;
	else if (!ft_strcmp(tab, "PORTAL"))
		type = PORTAL;
	else if (!ft_strcmp(tab, "DOOR_CLOSE"))
		type = DOOR_CLOSE;
	else if (!ft_strcmp(tab, "DOOR_OPEN"))
		type = DOOR_OPEN;
	return (type);
}