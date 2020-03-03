#include "doom_nukem.h"

void	ed_delete_mob(t_mob **mobs, t_mob *mob)
{
	t_mob	*m;
	t_mob	*tmp_prev;
	t_mob	*tmp_next;

	if (mobs && mob)
	{
		tmp_prev = NULL;
		m = *mobs;
		while (m)
		{
			if (m == mob)			
			{
				tmp_next = m->next;
				free(m);
				if (tmp_prev)
					tmp_prev->next = tmp_next;
				else
					*mobs = tmp_next;
				m = tmp_next;
			}
			else
			{
				tmp_prev = m;
				m = m->next;
			}
		}
	}
}

static	void ed_delete_object_polys(t_map *map, t_object *object)
{
	t_poly	*p;
	t_poly	*tmp_next;
	t_poly	*tmp_prev;
	int		i;
	int		poly_count;
	
	tmp_prev = NULL;
	p = map->polys;
	if (map)
	{
		if (p != object->poly)
		{
			while (p && p->next != object->poly)
				p = p->next;
			if (p != map->polys)
				tmp_prev = p;
			p = p->next;
		}
		i = 0;
		poly_count = ed_get_object_poly_count(object);
		while (p && i < poly_count)
		{
			if (!tmp_prev)
			{
				map->polys = p->next;
				free(p);
				p = map->polys;
			}
			else
			{
				tmp_next = p->next;
				free(p);
				tmp_prev->next = tmp_next;
				p = tmp_next;
			}
			i++;
		}
	}
}

void	ed_delete_obj(t_map *map, t_object **objects, t_object *object)
{
	t_object	*obj;
	t_object	*tmp_prev;
	t_object	*tmp_next;

	if (map && objects && object)
	{
		ed_delete_object_polys(map, object);
		tmp_prev = NULL;
		obj = *objects;
		while (obj)
		{
			if (obj == object)
			{
				tmp_next = obj->next;
				free(obj);
				if (tmp_prev)
					tmp_prev->next = tmp_next;
				else
					*objects = tmp_next;
				obj = tmp_next;
			}
			else
			{
				tmp_prev = obj;
				obj = obj->next;
			}	
		}
	}
}
