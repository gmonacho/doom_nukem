#include "doom_nukem.h"

/*
**	OA . v = d
**
**	OA' = OA + t
**
**	(OA' - t) . v = d
**	OA' . v - d - v . t = 0
**	Mon d est egal a -d dans cette equation du a la premiere relation
**	Resultat on enleve a d le produit scalaire du vecteur normal et
**	de la translation
*/




void				rotate_ray(t_cartesienne *ray, t_matrice matrice)
{
	double			save_vx;
	double			save_vy;

	save_vx = ray->vx * matrice._00 + ray->vy * matrice._10 + ray->vz * matrice._20;
	save_vy = ray->vx * matrice._01 + ray->vy * matrice._11 + ray->vz * matrice._21;
	ray->vz = ray->vx * matrice._02 + ray->vy * matrice._12 + ray->vz * matrice._22;
	ray->vx = save_vx;
	ray->vy = save_vy;
}

void				rotate_dot(t_fdot_3d *dot, t_matrice matrice)
{
	double			save_vx;
	double			save_vy;

	save_vx = dot->x * matrice._00 + dot->y * matrice._10 + dot->z * matrice._20;
	save_vy = dot->x * matrice._01 + dot->y * matrice._11 + dot->z * matrice._21;
	dot->z = dot->x * matrice._02 + dot->y * matrice._12 + dot->z * matrice._22;
	dot->x = save_vx;
	dot->y = save_vy;
}

void				rotate_all(t_sector *sector, t_matrice matrice)
{
	t_linedef		*line;

	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			rotate_dot(&(line->equation.v), matrice);
			rotate_dot(&(line->i), matrice);
			rotate_dot(&(line->j), matrice);
			
			rotate_dot(&(line->poly_3d[0]), matrice);
			rotate_dot(&(line->poly_3d[1]), matrice);
			rotate_dot(&(line->poly_3d[2]), matrice);
			rotate_dot(&(line->poly_3d[3]), matrice);
			line = line->next;
		}
		sector = sector->next;
	}
}

double				scalar_product(t_fdot_3d v1, t_fdot_3d v2)
{
	return ((double)(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z));
}

void				translate_all(t_sector *sector, t_fdot_3d translation)
{
	t_linedef		*line;

	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			// printf("dd %f\n", -scalar_product(line->equation.v, translation));
			line->equation.d -= scalar_product(line->equation.v, translation);
			// line->origin.x += translation.x;
			// line->origin.y += translation.y;
			line->origin.z += translation.z;
			line = line->next;
		}
		sector = sector->next;
	}
}
