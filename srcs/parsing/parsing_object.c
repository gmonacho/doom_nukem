#include "doom_nukem.h"

void    set_object_dots(t_object *object)
{
    object->poly->dots[0].x = object->pos.x - object->width_2;
    object->poly->dots[0].y = object->pos.y;
    object->poly->dots[0].z = object->pos.z - object->height_2;
    object->poly->dots[1].x = object->pos.x + object->width_2;
    object->poly->dots[1].y = object->pos.y;
    object->poly->dots[1].z = object->pos.z - object->height_2;
    object->poly->dots[2].x = object->pos.x + object->width_2;
    object->poly->dots[2].y = object->pos.y;
    object->poly->dots[2].z = object->pos.z + object->height_2;
    object->poly->dots[3].x = object->pos.x - object->width_2;
    object->poly->dots[3].y = object->pos.y;
    object->poly->dots[3].z = object->pos.z + object->height_2;
}

void	add_object(t_object **object)
{	
	t_object *new_object;
	
	new_object = (t_object *)ft_memalloc(sizeof(t_object));
	new_object->next = *object;
	*object = new_object;
}

int			add_poly_object(t_object *object, char *type)
{
    if (!(object->poly = (t_poly *)ft_memalloc(sizeof(t_poly))))
		return (1);
	set_object_dots(object);
	object->poly->object = object;
	if (ft_strcmp(type, "HEAL"))
		object->type = HEAL;
	else if (ft_strcmp(type, "ARMOR"))
		object->type = ARMOR;
	else if (ft_strcmp(type, "TP"))
		object->type = TP;
	else if (ft_strcmp(type, "GUN"))
		object->type = GUN;
	else if (ft_strcmp(type, "BULLET"))
		object->type = BULLET;
	return (0);
}

int			object_data(char **tab, t_object **object, int i)
{
	char	*type;

	type = NULL;
	add_object(object);
    (*object)->width = 50;
    (*object)->height = 50;
	while (ft_strchr(tab[i], '}') == NULL)
	{
        if (ft_strstr(tab[i], "type = "))
            type = ft_strdup(ft_strrchr(tab[i], '=') + 2);
        if (ft_strstr(tab[i], "texture = "))
            (*object)->texture = ft_strdup(ft_strrchr(tab[i], '=') + 2);
        if (ft_strstr(tab[i], "posx = "))
        	(*object)->pos.x = ft_atoi(ft_strrchr(tab[i], '=') + 1);
        if (ft_strstr(tab[i], "posy = "))
            (*object)->pos.y = ft_atoi(ft_strrchr(tab[i], '=') + 1);
        if (ft_strstr(tab[i], "posz = "))
            (*object)->pos.z = ft_atoi(ft_strrchr(tab[i], '=') + 1);
        if (ft_strstr(tab[i], "width = "))
            (*object)->width = ft_atoi(ft_strrchr(tab[i], '=') + 1);
        if (ft_strstr(tab[i], "height = "))
            (*object)->height = ft_atoi(ft_strrchr(tab[i], '=') + 1);
		i++;
	}
    (*object)->width_2 = (*object)->width / 2;
    (*object)->height_2 = (*object)->height / 2;
	if (add_poly_object(*object, type))
		return (1);
	return (0);
	//printf("O |posx = %f\n", object->pos.x);
	// printf("O |posy = %f\n", object->pos.y);
	// printf("\n");
}