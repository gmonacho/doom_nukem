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

void    add_poly_object(t_object *object)
{
    object->poly = (t_poly *)ft_memalloc(sizeof(t_poly));
    if (object->poly != NULL)
        set_object_dots(object);
}

void	object_data(char **tab, t_object **object, int i)
{
	add_object(object);
	while (ft_strchr(tab[i], '}') == NULL)
	{
        if (ft_strstr(tab[i], "type = "))
            (*object)->type = ft_atoi(ft_strrchr(tab[i], '=') + 1);
        if (ft_strstr(tab[i], "posx = "))
        	(*object)->pos.x = ft_atoi(ft_strrchr(tab[i], '=') + 1);
        if (ft_strstr(tab[i], "posy = "))
            (*object)->pos.y = ft_atoi(ft_strrchr(tab[i], '=') + 1);
        if (ft_strstr(tab[i], "posz = "))
            (*object)->pos.z = ft_atoi(ft_strrchr(tab[i], '=') + 1);
        if (ft_strstr(tab[i], "dir = "))
            (*object)->dir = ft_atoi(ft_strrchr(tab[i], '=') + 1);
        if (ft_strstr(tab[i], "width = "))
        {
            (*object)->width = ft_atoi(ft_strrchr(tab[i], '=') + 1);
            (*object)->width_2 = (*object)->width / 2;
        }
        if (ft_strstr(tab[i], "height = "))
        {
            (*object)->height = ft_atoi(ft_strrchr(tab[i], '=') + 1);
        	(*object)->height_2 = (*object)->height / 2;
        }
		i++;
	}
	add_poly_object(*object);
	//printf("O |posx = %f\n", object->pos.x);
	// printf("O |posy = %f\n", object->pos.y);
	// printf("\n");
}