#include "doom_nukem.h"

t_sector	*new_sector()
{
	t_sector *sector;

	if (!(sector = (t_sector*)ft_memalloc(sizeof(t_sector))))
		return (ret_null_perror("sector allocation failed in new_sector"));
	sector->name = "Unnamed";
	sector->color.selected_color = (SDL_Color){255, 0, 0, 255};
	return (sector);
}

t_sector	*new_void_sector(void)
{
	t_sector *sector;

	if (!(sector = (t_sector *)ft_memalloc(sizeof(t_sector))))
		return (ret_null_perror("sector allocation failed in new_void_sector"));
	return (sector);
}

void		add_sector(t_sector **sectors, t_sector *new_sector)
{
	new_sector->next = *sectors;
	*sectors = new_sector;
}

int			get_nb_sectors(t_sector *sector)
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