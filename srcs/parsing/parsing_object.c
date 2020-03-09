#include "doom_nukem.h"

static void    set_poly_dots_rotz_only(t_poly *poly, t_fdot_3d pos,\
												float width_2, float height_2)
{
    poly->dots_rotz_only[0].x = pos.x - width_2;
    poly->dots_rotz_only[0].y = pos.y;
    poly->dots_rotz_only[0].z = pos.z - height_2;
    poly->dots_rotz_only[1].x = pos.x + width_2;
    poly->dots_rotz_only[1].y = pos.y;
    poly->dots_rotz_only[1].z = pos.z - height_2;
    poly->dots_rotz_only[2].x = pos.x + width_2;
    poly->dots_rotz_only[2].y = pos.y;
    poly->dots_rotz_only[2].z = pos.z + height_2;
    poly->dots_rotz_only[3].x = pos.x - width_2;
    poly->dots_rotz_only[3].y = pos.y;
    poly->dots_rotz_only[3].z = pos.z + height_2;
	print_poly(poly, 1);
}

static int		add_object(t_object **object)
{
	t_object	*new_object;

	if (!(new_object = (t_object *)ft_memalloc(sizeof(t_object))))
		return (1);
	new_object->next = *object;
	new_object->collide = 1;
	new_object->visible = 1;
	new_object->width = 50;
	new_object->height = 50;
	new_object->light_coef = 1;
	new_object->data = 1;
	new_object->dir = 0;
	*object = new_object;
	return (0);
}

static int		add_poly_object_norm(t_object *object, char *type_str)
{
	if (type_str == NULL)
	{
		ft_putendl("Error: Type of object is wrong !");
		return (1);
	}
	if (!ft_strcmp(type_str, "HEAL"))
		object->type = HEAL;
	else if (!ft_strcmp(type_str, "ARMOR"))
		object->type = ARMOR;
	else if (!ft_strcmp(type_str, "TP"))
		object->type = TP;
	else if (!ft_strcmp(type_str, "GUN"))
		object->type = GUN;
	else if (!ft_strcmp(type_str, "BULLET"))
		object->type = BULLET;
	else if (!ft_strcmp(type_str, "GRAVITY_INV"))
		object->type = GRAVITY_INV;
	else if (!ft_strcmp(type_str, "LIGHT"))
		object->type = LIGHT;
	else if (!ft_strcmp(type_str, "BOX"))
	{
		object->type = BOX;
		if (set_box_object(object, object->pos_rotz_only,\
											object->width_2, object->height_2))
			return (1);
		return (0);
	}
	else if (!ft_strcmp(type_str, "DOOR"))
	{
		object->type = DOOR;
		set_door_object(object, object->pos_rotz_only,\
												object->width, object->height);
		return (0);
	}
	else
	{
		ft_putendl("Error: Type of object is wrong !");
		return (1);
	}
	if (!(object->poly = (t_poly *)ft_memalloc(sizeof(t_poly))))
		return (1);
	set_poly_dots_rotz_only(object->poly, object->pos_rotz_only,\
											object->width_2, object->height_2);
	object->poly->light_coef = object->light_coef;
	object->poly->object = object;
	object->poly->mob = NULL;
	object->poly->next = NULL;
	return (0);
}

int				object_data(char **tab, t_object **object, int i)
{
	char		*type;

	if (add_object(object))
		return (1);
	type = object_data_fill(tab, object, i);
	(*object)->width_2 = (*object)->width / 2;
	(*object)->height_2 = (*object)->height / 2;
	(*object)->data = 1;
	if (add_poly_object_norm(*object, type))
		return (1);
	ft_strdel(&type);
	return (0);
}
