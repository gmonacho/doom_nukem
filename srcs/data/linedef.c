#include "doom_nukem.h"

/*
**	Angle e [0, pi]
*/

void		add_linedef(t_linedef **lines, t_linedef *new_linedef)
{
	new_linedef->next = *lines;
	*lines = new_linedef;
}

t_linedef	*init_linedef(t_linedef *line)
{
	double	a;

	if (line->p2.x - line->p1.x)
	{
		// line->equation.isequation = 1;
		a = (line->p2.y - line->p1.y) /\
						(double)(line->p2.x - line->p1.x);
		// line->equation.b = line->p1.y - line->equation.a * line->p1.x;
		line->angle = atan(a);
		if (line->angle < 0)
			line->angle += M_PI;
	}
	else
	{
		// line->equation.isequation = 0;
		// line->equation.a = line->p1.x;
		line->angle = M_PI_2;
	}
	line->equation = (t_plan){(t_fdot_3d){cos(line->angle - M_PI_2),\
								        sin(line->angle - M_PI_2),\
								        0},\
                                0};
	line->equation.d = -(line->equation.v.x * line->p1.x + line->equation.v.y * line->p1.y);
	line->next = NULL;
	// printf("Equation : %f pi\t%f %f %f %f\n", line->angle / M_PI, line->equation_2.a, line->equation_2.b, line->equation_2.c, line->equation_2.d);
	return (line);
}

t_linedef	*new_linedef(t_line line, SDL_Surface *texture, Uint32 flags)
{
	t_linedef	*newline;
    double      a;

	if (!(newline = (t_linedef *)ft_memalloc(sizeof(t_linedef))))
		return (ret_null_perror("lines allocation failed in new_linedef"));
	newline->p1 = line.p1;
	newline->p2 = line.p2;
	if (line.p2.x - line.p1.x)
	{
		// newline->equation.isequation = 1;
		a = (line.p2.y - line.p1.y) /\
						(double)(line.p2.x - line.p1.x);
		// newline->equation.b = line.p1.y - newline->equation.a * line.p1.x;
		newline->angle = atan(a);
		if (newline->angle < 0)
			newline->angle += M_PI;
	}
	else
	{
		// newline->equation.isequation = 0;
		// newline->equation.a = line.p1.x;
		newline->angle = M_PI_2;
	}
    newline->equation = (t_plan){(t_fdot_3d){cos(newline->angle - M_PI_2),\
								            sin(newline->angle - M_PI_2),\
								            0},\
                                0};
	newline->equation.d = -(newline->equation.v.x * line.p1.x + newline->equation.v.y * line.p1.y);
	newline->texture = texture;
	newline->side = SIDE_RIGHT;
	newline->flags = flags;
	newline->id = 0;
	newline->next = NULL;
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