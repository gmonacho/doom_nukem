#include "doom_nukem.h"

/*
**	Probleme :

Apres rot : 0x0
Avant rot : 0x0
Apres rot : 0x0
Avant rot : 0x0
Apres rot : 0x0
Avant rot : 0x0
Apres rot : 0x0
Avant rot : 0x0
Apres rot : 0x7fc368e49480				//Apres copy_rotate_rotz_only
Col game loop
Is col : 0								//Aucun polys
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Col game loop
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Col game loop
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Col game loop
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Col game loop
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Col game loop
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Col game loop
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Col game loop
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Col game loop
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Col game loop
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Is col : 0
Collision avec 10 murs ??
Poly 1
D0 (360.550537, 88.336342, -45.000000)
D1 (-38.694489, 389.343384, -45.000000)
D2 (-339.702728, -9.901632, -45.000000)
D3 (59.543690, -310.908966, -45.000000)

0.000000 0.000000 25.000000 1125.000000
*/

static int		is_in_poly(t_poly *poly, t_fdot_3d dot)
{
	t_fdot		coord1;
	t_fdot_3d	dot_3d_in_plan;

	dot_3d_in_plan = fdot_3d_sub(dot, poly->dots_rotz_only[0]);
	coord1.x = scalar_product(dot_3d_in_plan, poly->i_rotz_only) / poly->ii;
	coord1.y = scalar_product(dot_3d_in_plan, poly->j_rotz_only) / poly->jj;
	return (!(coord1.x < 0 || 1 < coord1.x || coord1.y < 0 || 1 < coord1.y) ? 1 : 0);
}

static int		is_in_segment(t_fdot_3d is, t_fdot_3d d1, t_fdot_3d d2)
{
	if (!(is.x > d1.x || is.x > d2.x) ||\
		!(is.x < d1.x || is.x < d2.x))
		return (0);
	return (1);
}

int				poly_collision(t_player *player, t_poly *poly)
{
	t_fdot_3d	proj_ortho;

	proj_ortho_plan((t_fdot_3d){0, 0, 0}, poly->equation_rotz_only, &proj_ortho);
	// printf("Plan %f %f %f %f\n", poly->equation_rotz_only.v.x, poly->equation_rotz_only.v.y, poly->equation_rotz_only.v.z, poly->equation_rotz_only.d);
	// printf("Proj_ortho %f %f %f\n", proj_ortho.x, proj_ortho.y, proj_ortho.z);
	if (mag(proj_ortho) > player->width_2)
	{
		// printf("Poly trop loin\n");
		return (0);
	}
	if (is_in_poly(poly, proj_ortho))
	{
		// printf("Sphere's dot is in the Poly : proj ortho %f %f %f\n", proj_ortho.x, proj_ortho.y, proj_ortho.z);
		// printf("Eq : %f %f %f %f\n", poly->equation_rotz_only.v.x, poly->equation_rotz_only.v.y, poly->equation_rotz_only.v.z, poly->equation_rotz_only.d);
		// print_poly(poly, 1);
		return (1);
	}
	if (mag(poly->dots_rotz_only[0]) <= player->width_2 ||\
		mag(poly->dots_rotz_only[1]) <= player->width_2 ||\
		mag(poly->dots_rotz_only[2]) <= player->width_2 ||\
		mag(poly->dots_rotz_only[3]) <= player->width_2)
	{
		// printf("Poly's dot is in the Sphere\n");
		return (1);
	}
	if ((is_in_segment(proj_ortho_origin_line(poly->dots_rotz_only[0], poly->dots_rotz_only[1], &proj_ortho),\
						poly->dots_rotz_only[0], poly->dots_rotz_only[1]) &&\
		mag(proj_ortho) <= player->width_2) ||\
		(is_in_segment(proj_ortho_origin_line(poly->dots_rotz_only[1], poly->dots_rotz_only[2], &proj_ortho),\
						poly->dots_rotz_only[1], poly->dots_rotz_only[2]) &&\
		mag(proj_ortho) <= player->width_2 )||\
		(is_in_segment(proj_ortho_origin_line(poly->dots_rotz_only[2], poly->dots_rotz_only[3], &proj_ortho),\
						poly->dots_rotz_only[2], poly->dots_rotz_only[3]) &&\
		mag(proj_ortho) <= player->width_2) ||\
		(is_in_segment(proj_ortho_origin_line(poly->dots_rotz_only[3], poly->dots_rotz_only[0], &proj_ortho),\
						poly->dots_rotz_only[3], poly->dots_rotz_only[0]) &&\
		mag(proj_ortho) <= player->width_2))
	{
		// printf("Poly's segment is in the Sphere\n");
		return (1);
	}
	// printf("Poly is not in the Sphere\n");
	return (0);
}

t_poly			*collisions_sphere(t_map *map, t_player *player, t_poly *poly)
{
	// translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, player->_4_height_10});
	while (poly)
	{
		// print_poly(poly, 1);
		if (!poly->is_slide_ban)
		{
			if (poly_collision(player, poly))
			{
				// printf("COLLISION !!!\n");
				// printf("Is col : 1\n");
				// translate_all_rotz_only(map->polys, (t_dfdot_3d){0, 0, -player->_4_height_10});
				return (poly);
			}
			// printf("Is col : 0\n");
		}
		poly = poly->next;
	}
	// translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -player->_4_height_10});
	// printf("\n\n");
	map = NULL;
	return (NULL);
}