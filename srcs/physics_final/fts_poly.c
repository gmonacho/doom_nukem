#include "doom_nukem.h"

int			lstlen(t_poly *poly)
{
	int		count;

	count = 0;
	while (poly)
	{
		count++;
		poly = poly->next;
	}
	return (count);
}

void		copy_poly_lst(t_poly *dst, t_poly *src)
{
	t_poly	*tmp;

	while (src)
	{
		tmp = dst->next;
		ft_memcpy(dst, src, sizeof(t_poly));
		dst->next = tmp;
		dst = dst->next;
		src = src->next;
	}
}

int			create_poly_save(t_map *map)
{
	int		i;
	int		length;
	t_poly	*poly;

	if ((length = lstlen(map->polys)) &&\
		!(map->polys_save = (t_poly *)malloc(sizeof(t_poly))))
		return (1);
	poly = map->polys_save;
	i = 0;
	while (++i < length)
	{
		if (!(poly->next = (t_poly *)malloc(sizeof(t_poly))))
			return (1);
		poly = poly->next;
	}
	poly->next = NULL;
	return (0);
}
