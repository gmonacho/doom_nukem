#include "doom_nukem.h"

//Probleme si la collision est sur la tranche d'un plan
//Vaut mieux desactiver le slide dans ce cas
//Sinon pas realiste avec la hitbox

//Apres le while(collision) check si ya tjs une collision pour les is_slide_ban=1

void				slide(t_map *map, t_poly *polys, t_poly *polys_save, t_poly *poly_collide, int i)
{
	t_fdot_3d		dpos;
	t_plan			plan;
	t_fdot_3d		proj_ortho;
	t_cartesienne	proj_ortho_ray;

	dpos = (t_fdot_3d){polys_save->dots_rotz_only[0].x - polys->dots_rotz_only[0].x,\
						polys_save->dots_rotz_only[0].y - polys->dots_rotz_only[0].y,\
						polys_save->dots_rotz_only[0].z - polys->dots_rotz_only[0].z};
	plan = (t_plan){poly_collide->equation_rotz_only.v, 0};
	proj_ortho_ray = (t_cartesienne){dpos.x, dpos.y, dpos.z,\
									plan.v.x,\
									plan.v.y,\
									plan.v.z,\
									NULL, 0, 0, NULL};

	// if (is_null(scalar_product(plan.v, dpos), 0.005))
	// {
	// 	// copy_poly_lst(polys, polys_save);
	// 	poly_collide->is_slide_ban = 1;
	// 	// printf("< slide canceled >\n");
	// 	// printf("Plan %f %f %f %f\n", plan.v.x, plan.v.y, plan.v.z, plan.d);
	// 	// printf("Dpos o %f %f %f\n", dpos.x, dpos.y, dpos.z);
	// 	// printf("Dpos v %f %f %f\n", proj_ortho_ray.vx, proj_ortho_ray.vy, proj_ortho_ray.vz);
	// 	// print_poly(poly_collide, 1);
	// 	return ;
	// }
	if (!intersection_plan_ray(&proj_ortho, plan, proj_ortho_ray))
	{
		printf("CHELOU slide trouve pas de collision ???\n");
		exit(1);
	}
	// printf("Proj ortho %f %f %f\n", proj_ortho.x, proj_ortho.y, proj_ortho.z);
	// printf("Ray o/v %f %f %f\t%f %f %f\n", proj_ortho_ray.ox, proj_ortho_ray.oy, proj_ortho_ray.oz,\
	// 											proj_ortho_ray.vx, proj_ortho_ray.vy, proj_ortho_ray.vz);
	// printf("Proj :  %f %f %f\n", proj_ortho.x, proj_ortho.y, proj_ortho.z);

	copy_poly_lst(polys, polys_save);
	translate_all_rotz_only(polys, (t_fdot_3d){-proj_ortho.x,\
													-proj_ortho.y,\
													-proj_ortho.z});

	// if (is_null(poly_collide->equation_rotz_only.v.x, 0.0005) &&\
	// 	is_null(poly_collide->equation_rotz_only.v.y, 0.0005))
	// {
	// 	i = 0;
	// 	// print_poly(poly_collide, 1);
	// 	// printf("Colli : %d\n", is_poly_collision(&(map->player), poly_collide));
	// }

	if (poly_collision(&(map->player), poly_collide))
	{	//Probleme de collision : 
		//Detecte une col fausse donc tente de la reparer mais elle est tjs detecte
		printf("Tjs une col sur slidegt rths ef : %f %f %f\n", proj_ortho.x, proj_ortho.y, proj_ortho.z);
		// print_poly(poly_collide, 1);
		copy_poly_lst(polys, polys_save);
		// print_poly(poly_collide, 1);
		// if (poly_collision(&(map->player), poly_collide))
		// {
		// 	// printf("TJS UNE COLLISIONS WTF\n");
		// 	// map->player.debug = 1;
		// 	poly_collision(&(map->player), poly_collide);
		// }
	}

	// if (i == 4)
	// {
	// }
	i = 0;
	// translate_all_rotz_only(polys_save, (t_fdot_3d){-proj_ortho.x,\
	// 												-proj_ortho.y,\
	// 												-proj_ortho.z});
	// copy_poly_lst(polys, polys_save);

	// copy_rotate_rotz_only(polys_save, create_ry_matrix(-map->player.rot_y));
	map = NULL;
}

// void		slide(t_map *map, t_poly *polys, t_poly *polys_save, t_poly *poly_collide)
// {
// 	// t_poly			*poly_collide_saved;
// 	t_fdot_3d		dpos;
// 	t_cartesienne	dpos_ray;
// 	t_fdot_3d		collision;
// 	float			angle;
// 	t_fdot_3d		rot_axe;
// 	t_fdot_3d		rebond;

// 	// poly_collide_saved = return_saved_poly(polys_save, poly_collide->index);
// 	dpos = (t_fdot_3d){polys_save->dots_rotz_only[0].x - polys->dots_rotz_only[0].x,\
// 						polys_save->dots_rotz_only[0].y - polys->dots_rotz_only[0].y,\
// 						polys_save->dots_rotz_only[0].z - polys->dots_rotz_only[0].z};
// 	dpos_ray = (t_cartesienne){0, 0, 0, dpos.x, dpos.y, dpos.z, NULL, 0, 0, NULL};
// 	intersection_plan_ray(&collision, poly_collide->equation_rotz_only, dpos_ray);
// 	printf("Col %f %f %f\n", collision.x, collision.y, collision.z);

// 	proj_ortho_ray = (t_cartesienne){0, 0, 0,\
// 									poly_collide->equation_rotz_only.v.x,\
// 									poly_collide->equation_rotz_only.v.y,\
// 									poly_collide->equation_rotz_only.v.z,\
// 									NULL, 0, 0, NULL};
// 	intersection_plan_ray(&proj_ortho, poly_collide->equation_rotz_only, proj_ortho_ray);
// 	printf("Proj ortho %f %f %f\n", proj_ortho.x, proj_ortho.y, proj_ortho.z);

// 	angle = asin(mag(proj_ortho) / mag(dpos));
// 	rot_axe = vectoriel_product(dpos, poly_collide->equation_rotz_only.v);

// 	rebond = rotate_dot(dpos, create_matrix(rot_axe, 2 * angle));
// 	translate_all_rotz_only(polys, (t_fdot_3d){-(dpos.x + rebond.x),\
// 												-(dpos.y + rebond.y),\
// 												-(dpos.z + rebond.z)});
// 	copy_rotate_rotz_only(map->polys, create_ry_matrix(-map->player.rot_y));
// }