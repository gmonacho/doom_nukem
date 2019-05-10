#include "data.h"
#include "libft.h"
#include "ret_error.h"

void		add_linedef(t_linedef **lines, t_linedef *new_linedef)
{
	new_linedef->next = *lines;
	*lines = new_linedef;
}

t_linedef	*new_linedef(t_line line, SDL_Texture *p1p2, SDL_Texture *p2p1, Uint32 flags)
{
	t_linedef *linedef;

	if (!(linedef = (t_linedef*)ft_memalloc(sizeof(t_linedef))))
		return (ret_null_perror("linedef allocation failed in new_linedef"));
	linedef->p1 = line.p1;
	linedef->p2 = line.p2;
	linedef->p1p2_texture = p1p2;
	linedef->p2p1_texture = p2p1;
	linedef->flags = flags;
	linedef->next = NULL;
	return (linedef);
}

int			get_nb_linedef(t_linedef *lines)
{
	t_linedef 	*tmp;
	int			nb;

	nb = 0;
	tmp = lines;
	while (tmp)
	{
		nb++;
		tmp = tmp->next;
	}
	return (nb);
}