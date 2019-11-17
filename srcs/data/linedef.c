#include "doom_nukem.h"

/*
**	Angle e [0, pi]
*/

void		add_linedef(t_linedef **lines, t_linedef *new_linedef)
{
	new_linedef->next = *lines;
	*lines = new_linedef;
}

t_linedef	*init_equation(t_linedef *line)	//init_linedef() a la base
{
	double	alpha;

	alpha = line->p2.x - line->p1.x ? atan((line->p2.y - line->p1.y) /\
									(double)(line->p2.x - line->p1.x)) :\
									M_PI_2;
	if (alpha < 0)
		alpha += M_PI;
	line->equation = (t_plan){(t_fdot_3d){cos(alpha - M_PI_2),\
								        sin(alpha - M_PI_2),\
								        0},\
                                0};
	line->equation.d = -(line->equation.v.x * line->p1.x + line->equation.v.y * line->p1.y);
	printf("Equation : %f %f %f %f\n", line->equation.v.x, line->equation.v.y, line->equation.v.z, line->equation.d);
	return (line);
}

t_linedef	*new_linedef(t_line line, SDL_Surface *texture, Uint32 flags)
{
	t_linedef	*newline;

	if (!(newline = (t_linedef *)ft_memalloc(sizeof(t_linedef))))
		return (ret_null_perror("lines allocation failed in new_linedef"));
	newline->p1 = line.p1;
	newline->p2 = line.p2;
	newline->texture = texture;
	newline->side = SIDE_RIGHT;
	newline->flags = flags;
	newline->id = 0;
	init_equation(newline);
	return (newline);
}

void		free_linedef(t_linedef *linedef)
{
	if (linedef)
	{
		ft_strdel(&linedef->name);
		if (linedef->texture)
			SDL_FreeSurface(linedef->texture);
		free(linedef);
	}
	linedef = NULL; 
}

void		free_linedefs(t_linedef **lines)
{
	t_linedef	*l;
	t_linedef	*tmp_next;

	l = *lines;
	while (l)
	{
		tmp_next = l->next;
		free_linedef(l);
		l = tmp_next;
	}
	*lines = NULL;
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