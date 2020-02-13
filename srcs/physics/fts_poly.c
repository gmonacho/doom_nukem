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
	t_poly	*tmp_next;
	int		tmp_is_slide_ban;

	while (src)
	{
		tmp_next = dst->next;
		tmp_is_slide_ban = dst->is_slide_ban;
		ft_memcpy(dst, src, sizeof(t_poly));
		dst->next = tmp_next;
		dst->is_slide_ban = tmp_is_slide_ban;
		dst = dst->next;
		src = src->next;
	}
}

// void		copy_poly_lst(t_poly *dst, t_poly *src, t_object *dst_obj, t_object *src_obj)
// {
// 	// t_poly	*tmp_next;
// 	void	*tmp_next;
// 	int		tmp_is_slide_ban;

// 	while (src)
// 	{
// 		tmp_next = (void *)(dst->next);
// 		tmp_is_slide_ban = dst->is_slide_ban;
// 		ft_memcpy(dst, src, sizeof(t_poly));
// 		dst->next = (t_poly *)tmp_next;
// 		dst->is_slide_ban = tmp_is_slide_ban;
// 		dst = dst->next;
// 		src = src->next;
// 	}
// 	while (src_obj)
// 	{
// 		tmp_next = (void *)(dst_obj->next);
// 		ft_memcpy(dst_obj, src_obj, sizeof(t_object));
// 		dst_obj->next = (t_object *)tmp_next;
// 		dst_obj = dst_obj->next;
// 		src_obj = src_obj->next;
// 	}
// }

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
