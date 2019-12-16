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

static t_fdot_3d	return_rotate_dot(t_fdot_3d dot, t_matrix matrix)
{
	return ((t_fdot_3d){dot.x * matrix._00 + dot.y * matrix._10 + dot.z * matrix._20,\
						dot.x * matrix._01 + dot.y * matrix._11 + dot.z * matrix._21,\
						dot.x * matrix._02 + dot.y * matrix._12 + dot.z * matrix._22});
}

static void			rotate_dot(t_fdot_3d *dot, t_matrix matrix)
{
	float			save_vx;
	float			save_vy;

	save_vx = dot->x * matrix._00 + dot->y * matrix._10 + dot->z * matrix._20;
	save_vy = dot->x * matrix._01 + dot->y * matrix._11 + dot->z * matrix._21;
	dot->z = dot->x * matrix._02 + dot->y * matrix._12 + dot->z * matrix._22;
	dot->x = save_vx;
	dot->y = save_vy;
}

// void				rotate_all_dots(t_poly *poly, t_matrix matrix)
// {
// 	while (poly)
// 	{
// 		rotate_dot(&(poly->equation.v), matrix);
		
// 		rotate_dot(&(poly->dots[0]), matrix);
// 		rotate_dot(&(poly->dots[1]), matrix);
// 		rotate_dot(&(poly->dots[2]), matrix);
// 		rotate_dot(&(poly->dots[3]), matrix);

// 		poly->i = (t_fdot_3d){	poly->dots[1].x - poly->dots[0].x,\
// 								poly->dots[1].y - poly->dots[0].y,\
// 								poly->dots[1].z - poly->dots[0].z};
// 		poly->j = (t_fdot_3d){	poly->dots[N_DOTS_POLY - 1].x - poly->dots[0].x,\
// 								poly->dots[N_DOTS_POLY - 1].y - poly->dots[0].y,\
// 								poly->dots[N_DOTS_POLY - 1].z - poly->dots[0].z};
// 		// poly->i = (t_fdot_3d){	poly->dots[0].x - poly->dots[1].x,\
// 		// 						poly->dots[0].y - poly->dots[1].y,\
// 		// 						poly->dots[0].z - poly->dots[1].z};
// 		// poly->j = (t_fdot_3d){	poly->dots[0].x - poly->dots[N_DOTS_POLY - 1].x,\
// 		// 						poly->dots[0].y - poly->dots[N_DOTS_POLY - 1].y,\
// 		// 						poly->dots[0].z - poly->dots[N_DOTS_POLY - 1].z};
// 		poly = poly->next;
// 	}
// }

void				rotate_all_rotz_only(t_poly *poly, t_matrix matrix)
{
	while (poly)
	{
		rotate_dot(&(poly->equation_rotz_only.v), matrix);
		
		rotate_dot(&(poly->dots_rotz_only[0]), matrix);
		rotate_dot(&(poly->dots_rotz_only[1]), matrix);
		rotate_dot(&(poly->dots_rotz_only[2]), matrix);
		rotate_dot(&(poly->dots_rotz_only[3]), matrix);

		poly->i = (t_fdot_3d){	poly->dots_rotz_only[1].x - poly->dots_rotz_only[0].x,\
								poly->dots_rotz_only[1].y - poly->dots_rotz_only[0].y,\
								poly->dots_rotz_only[1].z - poly->dots_rotz_only[0].z};
		poly->j = (t_fdot_3d){	poly->dots_rotz_only[N_DOTS_POLY - 1].x - poly->dots_rotz_only[0].x,\
								poly->dots_rotz_only[N_DOTS_POLY - 1].y - poly->dots_rotz_only[0].y,\
								poly->dots_rotz_only[N_DOTS_POLY - 1].z - poly->dots_rotz_only[0].z};
		poly = poly->next;
	}
}

void				copy_rotate_rotz_only(t_poly *poly, t_matrix matrix)
{
	while (poly)
	{
		poly->equation.v = return_rotate_dot(poly->equation_rotz_only.v, matrix);
		
		poly->dots[0] = return_rotate_dot(poly->dots_rotz_only[0], matrix);
		poly->dots[1] = return_rotate_dot(poly->dots_rotz_only[1], matrix);
		poly->dots[2] = return_rotate_dot(poly->dots_rotz_only[2], matrix);
		poly->dots[3] = return_rotate_dot(poly->dots_rotz_only[3], matrix);

		poly->i = (t_fdot_3d){	poly->dots[1].x - poly->dots[0].x,\
								poly->dots[1].y - poly->dots[0].y,\
								poly->dots[1].z - poly->dots[0].z};
		poly->j = (t_fdot_3d){	poly->dots[N_DOTS_POLY - 1].x - poly->dots[0].x,\
								poly->dots[N_DOTS_POLY - 1].y - poly->dots[0].y,\
								poly->dots[N_DOTS_POLY - 1].z - poly->dots[0].z};
		poly = poly->next;
	}
}










static void			translate_dot(t_fdot_3d *dot, t_fdot_3d translation)
{
	// printf("\t\tDot %f %f %f\n", dot->x, dot->y, dot->z);
	dot->x += translation.x;
	dot->y += translation.y;
	dot->z += translation.z;
	// printf("\t\tDot %f %f %f\n", dot->x, dot->y, dot->z);
}

void				translate_all(t_poly *poly, t_fdot_3d translation)
{
	// printf("Trans %f %f %f\n", translation.x, translation.y, translation.z);
	while (poly)
	{	
		// printf("poly = %p\n", poly);
		// poly->equation.d -= scalar_product(poly->equation.v, translation);
		// poly->equation_rotz_only.d -= scalar_product(poly->equation_rotz_only.v, translation);

		
	// printf("\t\tDot %f %f %f\n", poly->dots[0].x, poly->dots[0].y, poly->dots[0].z);
		translate_dot(&(poly->dots[0]), translation);
		translate_dot(&(poly->dots[1]), translation);
		translate_dot(&(poly->dots[2]), translation);
		translate_dot(&(poly->dots[3]), translation);
		translate_dot(&(poly->dots_rotz_only[0]), translation);
		translate_dot(&(poly->dots_rotz_only[1]), translation);
		translate_dot(&(poly->dots_rotz_only[2]), translation);
		translate_dot(&(poly->dots_rotz_only[3]), translation);
	// printf("\t\tDot %f %f %f\n", poly->dots[0].x, poly->dots[0].y, poly->dots[0].z);

		// poly->i = (t_fdot_3d){	poly->d2.x - poly->d1.x,\
		// 						poly->d2.y - poly->d1.y,\
		// 						poly->d2.z - poly->d1.z};
		// poly->j = (t_fdot_3d){	poly->d3.x - poly->d1.x,\
		// 						poly->d3.y - poly->d1.y,\
		// 						poly->d3.z - poly->d1.z};
		poly->equation.d = -(poly->equation.v.x * poly->dots[0].x + poly->equation.v.y * poly->dots[0].y + poly->equation.v.z * poly->dots[0].z);
		poly->equation_rotz_only.d = -(poly->equation_rotz_only.v.x * poly->dots_rotz_only[0].x + poly->equation_rotz_only.v.y * poly->dots_rotz_only[0].y + poly->equation_rotz_only.v.z * poly->dots_rotz_only[0].z);
		poly = poly->next;
	}
}
