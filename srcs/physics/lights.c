#include "doom_nukem.h"

int				is_in_shadow(t_map *map, t_poly *poly_light, t_fdot_3d light, t_poly *poly_collision, t_fdot_3d collision, t_fdot_3d vector)
{
	t_poly			*poly;
	t_fdot_3d		intersec;
	t_fdot			coord;
	t_cartesienne		ray;

	// printf("Light %f, %f, %f\n", light.x, light.y, light.z);
	// printf("Ray %f, %f, %f\n", ray.x, ray.y, ray.z);
	ray = (t_cartesienne){light.x, light.y, light.z,\
							vector.x, vector.y, vector.z,\
							0, NULL, 0, (t_fdot_3d){}, NULL};
	poly = map->polys;
	while (poly)
	{
		if (poly != poly_light && poly != poly_collision && !(poly->object && poly->object->visible == 0))
		{
			// printf("Poly eq. %f %f %f %f\n", poly->equation.v.x, poly->equation.v.y, poly->equation.v.z, poly->equation.d);
			if (intersection_plan_ray(&intersec, poly->equation, ray))
			{
				// printf("Collision %f, %f, %f\n", intersec.x, intersec.y, intersec.z);
				if (is_in_poly(poly, &coord, intersec) &&\
					is_in_segment(intersec, light, collision))
				{
					// printf("Coord %f %f\n", coord.x, coord.y);
					// printf("In poly\n");
					return (1);
				}
				// printf("Collision %d %d\n", coord.x, coord.y);
			}
			else
			{
				// printf("Parrallele\n");
				return (1);
			}
		}
		poly = poly->next;
	}
	// printf("Not in shadow\n");
	return (0);
}

int					process_light(t_map *map, t_poly *poly, t_fdot_3d collision, int color)
{
	t_fdot_3d		ray;
	float			dist;
	float			sc_product;
	float			light_coef;
	t_object		*object;
	// float			best_light_coef;
	float			tmp;
	// int i = 0;

	tmp = -1;
	light_coef = 0;
	// best_light_coef = 0;
	object = map->objects;
	while (object)
	{
		if (object->type == LIGHT)
		{
			// printf("Poly collide %f, %f, %f\n", collision.x, collision.y, collision.z);
			//rien faire si scalar < 0
			// printf("Pos %f %f %f\n", object->pos.x, object->pos.y, object->pos.z);
			ray = fdot_3d_sub(collision, object->pos);
			if (is_in_shadow(map, object->poly, object->pos, poly, collision, ray))
			{
				light_coef += 0;
			}
			else
			{
				sc_product = fabs(scalar_product(poly->equation.v, normalize(ray)));
				if ((dist = mag(ray)) < 1000)
					light_coef += sc_product/* * object->data*/;
					// light_coef += sc_product * (1 - dist / 1000)/* * object->data*/;
			}
			// if (light_coef > best_light_coef)
				// best_light_coef = light_coef;
			// printf("LIGHT : %d\n", i++);
		}
		object = object->next;
	}
	// best_light_coef *= poly->light_coef;
	// printf("N lights : %d\n", i);
	// exit(0);
	if (light_coef > 1)
		light_coef = 1;
	return (0xFF000000 +\
			((int)((color >> 16 & 0xFF) * light_coef) << 16) +\
			((int)((color >> 8 & 0xFF) * light_coef) << 8) +\
			(int)((color & 0xFF) * light_coef));
}