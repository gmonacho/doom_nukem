#include "doom_nukem.h"

int		dist(t_dot p1, t_dot p2)
{
	t_dot	v;

	v.x = abs(p2.x - p1.x);
	v.y = abs(p2.y - p1.y);
	return (sqrt(v.x * v.x + v.y * v.y));
}

double	fdist(t_fdot p1, t_fdot p2)
{
	return (fabs(sqrt((p2.x - p1.x) * (p2.x - p1.x) +\
					(p2.y - p1.y) * (p2.y - p1.y))));
}

double	mag(t_vector vector)
{
	return (fabs(sqrt(vector.x * vector.x + vector.y * vector.y)));
}

int		sign(double nbr)
{
	return (nbr < 0 ? -1 : 1);
}
