#include "doom_nukem.h"

double		prop(double value, t_dot inter1, t_dot inter2)
{
	if (inter1.y == inter1.x)
		return ((inter2.y + inter2.x) / 2);
	return (inter2.x + ((value - inter1.x) / (inter1.y - inter1.x)) *\
			(inter2.y - inter2.x));
}

double  	modulo(double nbr, double mod)
{
        return (nbr - (int)(nbr / mod) * mod -\
                        (mod < 0 ? mod : 0));
}

void		draw_affine(t_win *win, t_affine function)
{
	draw_line(	win,\
				(t_dot){0,		function.b},\
				(t_dot){win->w,	function.a * win->w + function.b});
}

int			lines_intersection(t_fdot *intersection, t_affine *line1, t_affine *line2)
{
	// printf("Is : %d\n", line1->isequation);
	if (line1->isequation)
	{
		if (line2->isequation)
			intersection->x = (line1->b - line2->b) /\
						(double)(line2->a - line1->a);
		else
			intersection->x = line2->a;
		intersection->y = line1->a * intersection->x + line1->b;
		// printf("Diff a = %f\tDiff b = %f\tIntersectino x = %f\n", line2->a - line1->a, line1->b - line2->b, (line1->b - line2->b) /\
						// (double)(line2->a - line1->a));
	}
	else
	{
		if (line2->isequation)
			intersection->x = line1->a;
		else
			return (1);
		intersection->y = line2->a * intersection->x + line2->b;
	}
	return (0);
}
