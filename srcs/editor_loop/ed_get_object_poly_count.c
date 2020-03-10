#include "doom_nukem.h"

int			ed_get_object_poly_count(const t_object *obj)
{
	if (obj->type == BOX)
		return (6);
	else if (obj->type == DOOR)
		return (2);
	else
		return (1);
}
