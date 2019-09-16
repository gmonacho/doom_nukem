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
	new_sector->ceil_height = 1;
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

void	reverse_sectors(t_sector **sectors)
{
	t_sector	*start;
	t_sector	*s;
	t_sector	*moving;
	t_sector	*limit;

	start = *sectors;
	if (start->next)
	{
		s = start;
		moving  = NULL;
		while (s->next)
			s = s->next;
		limit = s;
		while (start != moving)
		{
			s = start;
			while (s->next != limit)
				s = s->next;
			moving = s;
			s = start;
			if (start != moving)
			{
				while (s->next != moving)
					s = s->next;
				s->next = limit;
			}
			s = limit;
			while (s->next)
				s = s->next;
			s->next = moving;
			moving->next = NULL;
		}
		*sectors = limit;
	}
}