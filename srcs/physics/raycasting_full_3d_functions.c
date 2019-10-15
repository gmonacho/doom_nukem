#include "doom_nukem.h"

double          fdist_3d(t_fdot_3d p1, t_fdot_3d p2)
{
    return (sqrt(   (p2.x - p1.x) * (p2.x - p1.x) +\
				    (p2.y - p1.y) * (p2.y - p1.y) +\
                    (p2.z - p1.z) * (p2.z - p1.z)));
}

int             sence(t_cartesienne ray, t_fdot_3d collision)
{
    return (fdist_3d(collision, (t_fdot_3d){ray.ox, ray.oy, ray.oz}) >\
            fdist_3d(collision, (t_fdot_3d){ray.ox + ray.vx,\
                                            ray.oy + ray.vy,\
                                            ray.oz + ray.vz,}) ? 1 : 0);
}

void			set_new_position_3d(t_fdot_3d *pos, t_linedef *line1, t_linedef *line2, t_sector **sector)
{
	double		p;

	// printf("New position\n");
	if (line1->p2.x == line1->p1.x)
		p = prop(pos->y,\
				(t_dot){line1->p1.y, line1->p2.y},\
				(t_dot){0, 10000});
	else
		p = prop(pos->x,\
				(t_dot){line1->p1.x, line1->p2.x},\
				(t_dot){0, 10000});
	// printf("Prop : %f\tposy = %f\tx = %d\ty = %d\n", p, pos->y, line1->p1.y, line1->p2.y);
	pos->x = prop(p,	(t_dot){0, 10000},\
						(t_dot){line2->p2.x, line2->p1.x});
	pos->y = prop(p,	(t_dot){0, 10000},\
						(t_dot){line2->p2.y, line2->p1.y});
	*sector = line2->sector;
}