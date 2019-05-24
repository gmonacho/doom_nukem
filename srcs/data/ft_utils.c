#include "doom_nukem.h"

/*
**	Une distance calcule avec sqrt doit tjs etre revoyer 
**	en double, a nous de la cast apres si on veut
*/

double		dist(t_dot p1, t_dot p2)
{
	return (sqrt((p2.x - p1.x) * (p2.x - p1.x) +\
					(p2.y - p1.y) * (p2.y - p1.y)));
}

double		fdist(t_fdot p1, t_fdot p2)
{
	return (sqrt((p2.x - p1.x) * (p2.x - p1.x) +\
					(p2.y - p1.y) * (p2.y - p1.y)));
}

double		mag(t_vector vector)
{
	return (sqrt(vector.x * vector.x + vector.y * vector.y));
}

double		fmag(t_fvector vector)
{
	return (sqrt(vector.x * vector.x + vector.y * vector.y));
}

int			sign(double nbr)
{
	return (nbr < 0 ? -1 : 1);
}

double		map(double value, t_dot inter1, t_dot inter2)
{
	if (inter1.y == inter1.x)
		return ((inter2.y + inter2.x) / 2);
	return (inter2.x + ((value - inter1.x) / (inter1.y - inter1.x)) *\
			(inter2.y - inter2.x));
}
