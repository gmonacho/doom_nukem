#include "doom_nukem.h"

/*
**	Une distance calcule avec sqrt doit tjs etre revoyer 
**	en double, a nous de la cast apres si on veut
*/

double  	modulo(double nbr, double mod)
{
	return (nbr - (int)(nbr / mod) * mod -\
					(mod < 0 ? mod : 0));
}
// double		dist(t_dot p1, t_dot p2)
// {
// 	return (sqrt((p2.x - p1.x) * (p2.x - p1.x) +\
// 					(p2.y - p1.y) * (p2.y - p1.y)));
// }

double		fdist(t_fdot p1, t_fdot p2)
{
	return (sqrt((p2.x - p1.x) * (p2.x - p1.x) +\
					(p2.y - p1.y) * (p2.y - p1.y)));
}

// double		mag(t_vector vector)
// {
// 	return (sqrt(vector.x * vector.x + vector.y * vector.y));
// }

// double		fmag(t_fvector vector)
// {
// 	return (sqrt(vector.x * vector.x + vector.y * vector.y));
// }

int			sign(double nbr)
{
	return (nbr < 0 ? -1 : 1);
}

void		normalize(double *angle)
{
	while (*angle < 0)
		*angle += _2_PI;
	while (*angle >= _2_PI)
		*angle -= _2_PI;
}
