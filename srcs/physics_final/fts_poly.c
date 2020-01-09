#include "doom_nukem.h"

void		tab_cpy(t_fdot_3d dst[N_DOTS_POLY * 2], t_fdot_3d src[N_DOTS_POLY * 2], int length)
{
	int		i;

	i = -1;
	while (++i < length)
		dst[i] = src[i];
}

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
		// printf("Poly cpy dest %p\tsrc %p\n", dst, src);
		tmp = dst->next;
		ft_memcpy(dst, src, sizeof(t_poly));
		dst->next = tmp;
		tab_cpy(dst->dots_rotz_only, src->dots_rotz_only, N_DOTS_POLY);
		tab_cpy(dst->dots, src->dots, N_DOTS_POLY);
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
