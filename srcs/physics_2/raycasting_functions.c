#include "doom_nukem.h"

/*
**	Sence : Return 1 si la collision est dans le sens du vecteur directeur du rayon
**			Si la collision et l'origin du rayon sont confondu return 0
*/

double          fdist_3d(t_fdot_3d p1, t_fdot_3d p2)
{
    return (sqrt(   (p2.x - p1.x) * (p2.x - p1.x) +\
				    (p2.y - p1.y) * (p2.y - p1.y) +\
                    (p2.z - p1.z) * (p2.z - p1.z)));
}

int             sence(t_cartesienne ray, t_fdot_3d collision)
{
	// return (scalar_product((t_fdot_3d){ray->vx, ray->vy, ray->vz}, collision) > 0);
	if (ray.vx)
	{
		if (collision.x < ray.ox && ray.ox + ray.vx < ray.ox)
			return (1);
		else if (collision.x > ray.ox && ray.ox + ray.vx > ray.ox)
			return (1);
		else
			return (0);
	}
	else if (ray.vy)
	{
		if (collision.y < ray.oy && ray.oy + ray.vy < ray.oy)
			return (1);
		else if (collision.y > ray.oy && ray.oy + ray.vy > ray.oy)
			return (1);
		else
			return (0);
	}
	else
	{
		if (collision.z < ray.oz && ray.oz + ray.vz < ray.oz)
			return (1);
		else if (collision.z > ray.oz && ray.oz + ray.vz > ray.oz)
			return (1);
		else
			return (0);
	}
}

void			teleportation_ray(t_cartesienne *ray, t_linedef *line1, t_linedef *line2, t_sector **sector)
{
	double		p;

	// printf("New position : %p %p\n", line1, line2);
	if (line1->p2.x == line1->p1.x)
		p = prop(ray->oy,\
				(t_dot){line1->p1.y, line1->p2.y},\
				(t_dot){0, 10000});
	else
		p = prop(ray->ox,\
				(t_dot){line1->p1.x, line1->p2.x},\
				(t_dot){0, 10000});
	// printf("Prop : %f\tposy = %f\tx = %d\ty = %d\n", p, pos->y, line1->p1.y, line1->p2.y);
	ray->ox = prop(p,	(t_dot){0, 10000},\
						(t_dot){line2->p2.x, line2->p1.x});
	ray->oy = prop(p,	(t_dot){0, 10000},\
						(t_dot){line2->p2.y, line2->p1.y});
	*sector = line2->sector;
}