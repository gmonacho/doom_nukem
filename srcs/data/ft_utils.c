#include "doom_nukem.h"

double	dist(t_fdot d1, t_fdot d2)
{
	return (fabs(sqrt((d2.x - d1.x) * (d2.x - d1.x) +\
					(d2.y - d1.y) * (d2.y - d1.y))));
}

double	mag(t_vector vector)
{
	return (sqrt(vector.x * vector.x + vector.y * vector.y));
}
