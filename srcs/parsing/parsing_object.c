#include "doom_nukem.h"

int		add_object(t_object **object)
{	
	t_object *new_object;
	
	new_object = (t_object *)ft_memalloc(sizeof(t_object));
	new_object->next = *object;
	*object = new_object;
	return (0);
}

void	object_data(char **tab, t_object *object, int i)
{
	add_object(&object);
	while (ft_strchr(tab[i], '}') == NULL)
	{
		if (ft_strstr(tab[i], "posx ="))
			object->pos.x = ft_atoi(ft_strrchr(tab[i], '=') + 1);
		if (ft_strstr(tab[i], "posy ="))
			object->pos.y = ft_atoi(ft_strrchr(tab[i], '=') + 1);
		if (ft_strstr(tab[i], "sector ="))
			object->sector = ft_atoi(ft_strrchr(tab[i], '=') + 1);
		if (ft_strstr(tab[i], "id_texture ="))				
			object->id_texture = ft_atoi(ft_strrchr(tab[i], '=') + 1);
		i++;
	}
	printf("posx = %f\n", object->pos.x);
	printf("posy = %f\n", object->pos.y);
	printf("Sector = %d\n", object->sector);
	printf("id_texture = %d\n", object->id_texture);
	printf("\n");
}