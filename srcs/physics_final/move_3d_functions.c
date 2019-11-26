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

// void				rotate_ray(t_cartesienne *ray, t_matrice matrice)
// {
// 	double			save_vx;
// 	double			save_vy;

// 	save_vx = ray->vx * matrice._00 + ray->vy * matrice._10 + ray->vz * matrice._20;
// 	save_vy = ray->vx * matrice._01 + ray->vy * matrice._11 + ray->vz * matrice._21;
// 	ray->vz = ray->vx * matrice._02 + ray->vy * matrice._12 + ray->vz * matrice._22;
// 	ray->vx = save_vx;
// 	ray->vy = save_vy;
// }

static void			rotate_dot(t_fdot_3d *dot, t_matrice matrice)
{
	double			save_vx;
	double			save_vy;

	save_vx = dot->x * matrice._00 + dot->y * matrice._10 + dot->z * matrice._20;
	save_vy = dot->x * matrice._01 + dot->y * matrice._11 + dot->z * matrice._21;
	dot->z = dot->x * matrice._02 + dot->y * matrice._12 + dot->z * matrice._22;
	dot->x = save_vx;
	dot->y = save_vy;
}

void				rotate_all(t_poly *poly, t_matrice matrice)
{
	while (poly)
	{
		rotate_dot(&(poly->equation.v), matrice);
		
		rotate_dot(&(poly->dots[0]), matrice);
		rotate_dot(&(poly->dots[1]), matrice);
		rotate_dot(&(poly->dots[2]), matrice);
		rotate_dot(&(poly->dots[3]), matrice);

		poly->i = (t_fdot_3d){	poly->dots[0].x - poly->dots[1].x,\
								poly->dots[0].y - poly->dots[1].y,\
								poly->dots[0].z - poly->dots[1].z};
		poly->j = (t_fdot_3d){	poly->dots[0].x - poly->dots[N_DOTS_POLY - 1].x,\
								poly->dots[0].y - poly->dots[N_DOTS_POLY - 1].y,\
								poly->dots[0].z - poly->dots[N_DOTS_POLY - 1].z};
		poly = poly->next;
	}
}

static void			translate_dot(t_fdot_3d *dot, t_fdot_3d translation)
{
	dot->x += translation.x;
	dot->y += translation.y;
	dot->z += translation.z;
}

void				translate_all(t_poly *poly, t_fdot_3d translation)
{
	while (poly)
	{
		poly->equation.d -= scalar_product(poly->equation.v, translation);

		translate_dot(&(poly->dots[0]), translation);
		translate_dot(&(poly->dots[1]), translation);
		translate_dot(&(poly->dots[2]), translation);
		translate_dot(&(poly->dots[3]), translation);

		// poly->i = (t_fdot_3d){	poly->d2.x - poly->d1.x,\
		// 						poly->d2.y - poly->d1.y,\
		// 						poly->d2.z - poly->d1.z};
		// poly->j = (t_fdot_3d){	poly->d3.x - poly->d1.x,\
		// 						poly->d3.y - poly->d1.y,\
		// 						poly->d3.z - poly->d1.z};
		poly = poly->next;
	}
}
