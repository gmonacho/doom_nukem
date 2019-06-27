#include "doom_nukem.h"

void			add_sector(t_sector **sectors)
{
	t_sector	*new_sector;

	if (!(new_sector = (t_sector *)ft_memalloc(sizeof(t_sector))))
		return (ft_putendl("sector allocation failed in new_sector"));
	new_sector->color.selected_color = (SDL_Color){255, 0, 0, 255};
	if (!(new_sector->name = (char*)ft_memalloc(sizeof(char) * 30)))
		return (ft_putendl("allocation failed in add_sector"));
	ft_strcpy(new_sector->name, "sector");
	new_sector->color.pos = 0;
	new_sector->floor_height = 0;
	new_sector->floor_texture = NULL;
	new_sector->ceil_height = 0;
	new_sector->ceil_texture = NULL;
	new_sector->light_level = 0;
	new_sector->lines = NULL;
	new_sector->next = *sectors;
	*sectors = new_sector;
}

int				get_nb_sectors(t_sector *sector)
{
	t_sector	*s;
	int			i;

	s = sector;
	i = 0;
	while (s)
	{
		i++;
		s = s->next;
	}
	return (i);
}