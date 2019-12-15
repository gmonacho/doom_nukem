#include "doom_nukem.h"

/*
**	Une distance calcule avec sqrt doit tjs etre revoyer 
**	en float, a nous de la cast apres si on veut
*/

float  	modulo(float nbr, float mod)
{
	if (nbr < 0)
		return (mod - modulo(-nbr, mod));
	return (nbr - (int)(nbr / mod) * mod -\
					(mod < 0 ? mod : 0));
}

// float		fdist(t_fdot p1, t_fdot p2)
// {
// 	return (sqrt((p2.x - p1.x) * (p2.x - p1.x) +\
// 					(p2.y - p1.y) * (p2.y - p1.y)));
// }

float          fdist_3d(t_fdot_3d p1, t_fdot_3d p2)
{
    return (sqrt(   (p2.x - p1.x) * (p2.x - p1.x) +\
				    (p2.y - p1.y) * (p2.y - p1.y) +\
                    (p2.z - p1.z) * (p2.z - p1.z)));
}

float			fdist_3d_squared(t_fdot_3d p1, t_fdot_3d p2)
{
    return ((p2.x - p1.x) * (p2.x - p1.x) +\
			(p2.y - p1.y) * (p2.y - p1.y) +\
            (p2.z - p1.z) * (p2.z - p1.z));
}

int			sign(float nbr)
{
	return (nbr < 0 ? -1 : 1);
}

// void		normalize(float *angle)
// {
// 	while (*angle < 0)
// 		*angle += _2_PI;
// 	while (*angle >= _2_PI)
// 		*angle -= _2_PI;
// }

int			is_null(float nbr, float precision)
{
	return (!(nbr > precision || nbr < -precision) ? 1 : 0);
}
