#include "doom_nukem.h"

// void    set_poly_dots_rotz_only(t_object *object)
static void    set_poly_dots_rotz_only(t_poly *poly, t_fdot_3d pos, float width_2, float height_2)
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
	printf("pos %f %f %f\n", pos.x, pos.y, pos.z);
}

void			add_object(t_object **object)
{
	t_object *new_object;

	new_object = (t_object *)ft_memalloc(sizeof(t_object));
	new_object->next = *object;
	new_object->collide = 1;
	new_object->visible = 1;
	new_object->width = 50;
	new_object->height = 50;
	*object = new_object;
}

void			add_poly_object_norm(t_object *object, char *type_str)
{
	if (type_str == NULL)
		ft_putendl("Error: Type of object is wrong !");
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
	else if (!ft_strcmp(type_str, "BOX"))
	{
		object->type = BOX;
		set_box_object(object, object->pos_rotz_only, object->width_2, object->height_2);
		return ;
	}
	else if (!ft_strcmp(type_str, "LIGHT"))
		object->type = LIGHT;
	else
		ft_putendl("Error: Type of object is wrong !");
	set_poly_dots_rotz_only(object->poly, object->pos_rotz_only, object->width_2, object->height_2);
}

int				add_poly_object(t_object *object, char *type_str)
{
	add_poly_object_norm(object, type_str);
	printf("BOOOX pos %f %f %f\n", object->pos_rotz_only.x, object->pos_rotz_only.y, object->pos_rotz_only.z);
	if (!(object->poly = (t_poly *)ft_memalloc(sizeof(t_poly))))
		return (1);
	object->poly->object = object;
	object->poly->next = NULL;
	return (0);
}

int				object_data(char **tab, t_object **object, int i)
{
	char	*type;

	type = NULL;
	add_object(object);
	type = object_data_fill(tab, object, i);
	(*object)->width_2 = (*object)->width / 2;
	(*object)->height_2 = (*object)->height / 2;
	if (add_poly_object(*object, type))
		return (1);
	free(type);
	return (0);
}
