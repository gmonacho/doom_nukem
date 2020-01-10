#include "doom_nukem.h"

//Opti utiliser une autre ft que celle du raytracer pour pas avoir (ray->vx = 1) dans les eq.

// static t_poly		*return_saved_poly(t_poly *polys_save, int i)
// {
// 	int		j;

// 	j = -1;
// 	while (++j < i)
// 		polys_save = polys_save->next;
// 	return (polys_save);
// }

void		slide(t_map *map, t_poly *polys, t_poly *polys_save, t_poly *poly_collide)
{
	// t_poly			*poly_collide_saved;
	t_fdot_3d		dpos;
	t_cartesienne	dpos_ray;
	t_fdot_3d		proj_ortho;
	t_cartesienne	proj_ortho_ray;
	t_fdot_3d		collision;
	float			angle;
	t_fdot_3d		rot_axe;
	t_fdot_3d		rebond;

	// poly_collide_saved = return_saved_poly(polys_save, poly_collide->index);
	dpos = (t_fdot_3d){polys_save->dots_rotz_only[0].x - polys->dots_rotz_only[0].x,\
						polys_save->dots_rotz_only[0].y - polys->dots_rotz_only[0].y,\
						polys_save->dots_rotz_only[0].z - polys->dots_rotz_only[0].z};
	dpos_ray = (t_cartesienne){0, 0, 0, dpos.x, dpos.y, dpos.z, NULL, 0, 0, NULL};
	intersection_plan_ray(&collision, poly_collide->equation_rotz_only, dpos_ray);

	proj_ortho_ray = (t_cartesienne){0, 0, 0,\
									poly_collide->equation_rotz_only.v.x,\
									poly_collide->equation_rotz_only.v.y,\
									poly_collide->equation_rotz_only.v.z,\
									NULL, 0, 0, NULL};
	intersection_plan_ray(&proj_ortho, poly_collide->equation_rotz_only, proj_ortho_ray);

	angle = asin(mag(proj_ortho) / mag(dpos));
	rot_axe = ret_vectoriel_product(dpos, poly_collide->equation_rotz_only.v);

	rebond = return_rotate_dot(dpos, create_matrix(rot_axe, 2 * angle));
	translate_all_rotz_only(polys, (t_fdot_3d){-(dpos.x + rebond.x),\
												-(dpos.y + rebond.y),\
												-(dpos.z + rebond.z)});
	copy_rotate_rotz_only(map->polys, create_ry_matrix(-map->player.rot_y));
}