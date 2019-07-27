#include "doom_nukem.h"

void		add_linedef(t_linedef **lines, t_linedef *new_linedef)
{
	new_linedef->next = *lines;
	*lines = new_linedef;
}

t_linedef	*init_linedef(t_linedef *line)
{
	if (line->p2.x - line->p1.x)
	{
		line->equation.isequation = 1;
		line->equation.a = (line->p2.y - line->p1.y) /\
						(double)(line->p2.x - line->p1.x);
		line->equation.b = line->p1.y - line->equation.a * line->p1.x;
		line->angle = atan(line->equation.a);
	}
	else
	{
		line->equation.isequation = 0;
		line->equation.a = line->p1.x;
		line->angle = M_PI_2;
	}
	line->next = NULL;
	return (line);
}

t_linedef	*new_linedef(t_line line, SDL_Surface *texture, Uint32 flags)
{
	t_linedef	*newline;

	if (!(newline = (t_linedef *)ft_memalloc(sizeof(t_linedef))))
		return (ret_null_perror("lines allocation failed in new_linedef"));
	newline->p1 = line.p1;
	newline->p2 = line.p2;
	if (line.p2.x - line.p1.x)
	{
		newline->equation.isequation = 1;
		newline->equation.a = (line.p2.y - line.p1.y) /\
						(double)(line.p2.x - line.p1.x);
		newline->equation.b = line.p1.y - newline->equation.a * line.p1.x;
		newline->angle = atan(newline->equation.a);
	}
	else
	{
		newline->equation.isequation = 0;
		newline->equation.a = line.p1.x;
		newline->angle = M_PI_2;
	}
	newline->texture = texture;
	newline->flags = flags;
	newline->id = 0;
	newline->next = NULL;
	return (newline);
}

t_linedef	*new_void_linedef(void)
{
	t_linedef	*line;

	if (!(line = (t_linedef *)ft_memalloc(sizeof(t_linedef))))
		return (ret_null_perror("lines allocation failed in new_void_linedef"));
	return (line);
}

int			get_nb_linedef(t_linedef *lines, Uint32 flags)
{
	t_linedef	*tmp;
	int			nb;

	nb = 0;
	tmp = lines;
	while (tmp)
	{
		if (tmp->flags & flags)
			nb++;
		tmp = tmp->next;
	}
	return (nb);
}

void		add_linedef_flags(t_linedef **lines, Uint32 flags)
{
	t_linedef	*l;

	l = *lines;
	while (l)
	{
		l->flags |= flags;
		l = l->next;
	}
}