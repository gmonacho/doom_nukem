#include "libft.h"
#include <fcntl.h>
#include "data.h"
#include "ret_error.h"

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

t_sector	*new_void_sector(void)
{
	t_sector *sector;

	if (!(sector = (t_sector *)ft_memalloc(sizeof(t_sector))))
		exit(0);
	return (sector);
}
