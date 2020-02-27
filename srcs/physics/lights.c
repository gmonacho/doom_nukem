#include "doom_nukem.h"

int					is_in_shadow(t_map *map, t_poly *poly_light, t_fdot_3d light,\
									t_poly *poly_collision, t_fdot_3d collision, t_fdot_3d vector)
{
	t_poly			*poly;
	t_fdot_3d		intersec;
	t_fdot			coord;
	t_cartesienne	ray;

	ray = (t_cartesienne){light.x, light.y, light.z,\
							vector.x, vector.y, vector.z,\
							0, NULL, 0, (t_fdot_3d){}, NULL};
	poly = map->polys;
	while (poly)
	{
		if (poly != poly_light && poly != poly_collision &&
			!(poly->object &&\
			((poly->object->type == DOOR && poly->visible == 0) ||\
			(poly->object->type != DOOR && poly->object->visible == 0))) &&\
			intersection_plan_ray(&intersec, poly->equation, ray) &&\
			is_in_segment(intersec, light, collision) &&\
			is_in_poly(poly, &coord, intersec))
			return (1);
		poly = poly->next;
	}
	return (0);
}

int					compute_light_coef(t_map *map, t_poly *poly,\
										t_fdot_3d collision)
{
	t_fdot_3d		ray;
	t_object		*object;
	float			light_coef;

	light_coef = 0;
	object = map->objects;
	while (object)
	{
		if (object->type == LIGHT &&\
			!is_in_shadow(map, object->poly, object->pos, poly,\
						collision, ray = fdot_3d_sub(collision, object->pos)))
			light_coef += fabs(scalar_product(poly->equation.v,\
											normalize(ray))) * object->data;
		object = object->next;
	}
	if ((light_coef *= poly->light_coef) > 1)
		light_coef = 1;
	return (light_coef);
}

int					process_light(t_map *map, t_poly *poly,\
									t_fdot_3d collision, int color)
{
	float			light_coef;

	if (!poly)
		return (color);
	light_coef = map->view & LIGHT_VIEW ?\
					compute_light_coef(map, poly, collision) :\
					poly->light_coef;
	return (0xFF000000 +\
			((int)((color >> 16 & 0xFF) * light_coef) << 16) +\
			((int)((color >> 8 & 0xFF) * light_coef) << 8) +\
			(int)((color & 0xFF) * light_coef));
}