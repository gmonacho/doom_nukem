// #include "doom_nukem.h"

// /*
// **	Probleme :

// Apres rot : 0x0
// Avant rot : 0x0
// Apres rot : 0x0
// Avant rot : 0x0
// Apres rot : 0x0
// Avant rot : 0x0
// Apres rot : 0x0
// Avant rot : 0x0
// Apres rot : 0x7fc368e49480				//Apres copy_rotate_rotz_only
// Col game loop
// Is col : 0								//Aucun polys
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Col game loop
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Col game loop
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Col game loop
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Col game loop
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Col game loop
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Col game loop
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Col game loop
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Col game loop
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Col game loop
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Is col : 0
// Collision avec 10 murs ??
// Poly 1
// D0 (360.550537, 88.336342, -45.000000)
// D1 (-38.694489, 389.343384, -45.000000)
// D2 (-339.702728, -9.901632, -45.000000)
// D3 (59.543690, -310.908966, -45.000000)

// 0.000000 0.000000 25.000000 1125.000000
// */

// static int		is_in_poly(t_poly *poly, t_fdot_3d dot)
// {
// 	t_fdot		coord1;
// 	t_fdot_3d	dot_3d_in_plan;

// 	dot_3d_in_plan = fdot_3d_sub(dot, poly->dots_rotz_only[0]);
// 	coord1.x = scalar_product(dot_3d_in_plan, poly->i_rotz_only) / poly->ii;
// 	coord1.y = scalar_product(dot_3d_in_plan, poly->j_rotz_only) / poly->jj;
// 	return (!(coord1.x < 0 || 1 < coord1.x || coord1.y < 0 || 1 < coord1.y) ? 1 : 0);
// }

// static int		is_in_segment(t_fdot_3d is, t_fdot_3d d1, t_fdot_3d d2)
// {
// 	if (!(is.x > d1.x || is.x > d2.x) ||\
// 		!(is.x < d1.x || is.x < d2.x))
// 		return (0);
// 	return (1);
// }

// int				collision_poly_dots_2(t_map *map, t_fdot_3d dots[N_DOTS_POLY], float ray)
// {
// 	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, map->player._4_height_10});
// 	if (mag(dots[0]) <= ray ||\
// 		mag(dots[1]) <= ray ||\
// 		mag(dots[2]) <= ray ||\
// 		mag(dots[3]) <= ray)
// 	{
// 		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 		return (1);
// 	}
// 	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 	return (0);
// }

// static int		collision_poly_dots(t_map *map, t_fdot_3d dots[N_DOTS_POLY], float dist_min[N_DOTS_POLY], float ray)
// {
// 	if ((mag(dots[0]) <= ray && dist_min[0] < dist_min[3]) ||\
// 		(mag(dots[1]) <= ray && dist_min[0] < dist_min[1]))
// 	{
// 		printf("Poly's dot 01 is in the Sphere : %f\t%f\n", mag(dots[0]), mag(dots[1]));
// 		printf("Dist proj ortho 301 : %f\t%f\t%f\n", dist_min[3], dist_min[0], dist_min[1]);
// 		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 		return (0b0001);
// 		// return (0b1101);
// 	}
// 	if ((mag(dots[1]) <= ray && dist_min[1] < dist_min[0]) ||\
// 		(mag(dots[2]) <= ray && dist_min[1] < dist_min[2]))
// 	{
// 		printf("Poly's dot 12 is in the Sphere : %f\t%f\n", mag(dots[1]), mag(dots[2]));
// 		printf("Dist proj ortho 012 : %f\t%f\t%f\n", dist_min[0], dist_min[1], dist_min[2]);
// 		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 		return (0b0110);
// 		// return (0b0010);
// 	}
// 	if ((mag(dots[2]) <= ray && dist_min[2] < dist_min[1]) ||\
// 		(mag(dots[3]) <= ray && dist_min[2] < dist_min[3]))
// 	{
// 		printf("Poly's dot 23 is in the Sphere : %f\t%f\n", mag(dots[2]), mag(dots[3]));
// 		printf("Dist proj ortho 123 : %f\t%f\t%f\n", dist_min[1], dist_min[2], dist_min[3]);
// 		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 		return (0b1011);
// 		// return (0b0111);
// 	}
// 	if ((mag(dots[3]) <= ray && dist_min[3] < dist_min[2]) ||\
// 		(mag(dots[0]) <= ray && dist_min[3] < dist_min[0]))
// 	{
// 		printf("Poly's dot 30 is in the Sphere : %f\t%f\n", mag(dots[3]), mag(dots[0]));
// 		printf("Dist proj ortho 230 : %f\t%f\t%f\n", dist_min[2], dist_min[3], dist_min[0]);
// 		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 		return (0b1100);
// 		// return (0b1000);
// 	}
// 	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 	return (0);
// }

// int				collision_segment(t_map *map, t_fdot_3d dots[4], float width_2)
// {
// 	t_fdot_3d	proj_ortho[N_DOTS_POLY];
// 	float		dist_min[N_DOTS_POLY];

// 	// printf("Dot 0 : %f %f %f\n", dots[0].x, dots[0].y, dots[0].z);
// 	// printf("Dot 1 : %f %f %f\n", dots[1].x, dots[1].y, dots[1].z);
// 	// printf("Dot 2 : %f %f %f\n", dots[2].x, dots[2].y, dots[2].z);
// 	// printf("Dot 3 : %f %f %f\n", dots[3].x, dots[3].y, dots[3].z);

// 	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, map->player._4_height_10});
// 	if (is_in_segment(proj_ortho_origin_line(dots[0], dots[1], proj_ortho),\
// 						dots[0], dots[1]) &&\
// 		(dist_min[0] = mag(proj_ortho[0])) <= width_2)
// 	{
// 		printf("Poly's segment is in the Sphere, code 0b0001\n");
// 		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 		return (0b0001);
// 	}
// 	if (is_in_segment(proj_ortho_origin_line(dots[1], dots[2], proj_ortho + 1),\
// 						dots[1], dots[2]) &&\
// 		(dist_min[1] = mag(proj_ortho[1])) <= width_2)
// 	{
// 		printf("Poly's segment is in the Sphere, code 0b0110\n");
// 		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 		return (0b0110);
// 	}
// 	if (is_in_segment(proj_ortho_origin_line(dots[2], dots[3], proj_ortho + 2),\
// 						dots[2], dots[3]) &&\
// 		(dist_min[2] = mag(proj_ortho[2])) <= width_2)
// 	{
// 		printf("Poly's segment is in the Sphere, code 0b1011\n");
// 		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 		return (0b1011);
// 	}
// 	if (is_in_segment(proj_ortho_origin_line(dots[3], dots[0], proj_ortho + 3),\
// 						dots[3], dots[0]) &&\
// 		(dist_min[3] = mag(proj_ortho[3])) <= width_2)
// 	{
// 		printf("Poly's segment is in the Sphere, code 0b1100\n");
// 		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 		return (0b1100);
// 	}
// 	printf("Poly is not in the Sphere\n");
// 	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 	return (collision_poly_dots(map, dots, dist_min, width_2));
// 	// return (collision_poly_dots_2(map, dots, width_2));
// 	// return (0);
// }


// int				collision_poly(t_map *map, t_player *player, t_poly *poly)
// {
// 	t_fdot_3d	proj_ortho;

// 	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, map->player._4_height_10});
// 	proj_ortho_plan((t_fdot_3d){0, 0, 0}, poly->equation_rotz_only, &proj_ortho);
// 	// printf("Plan %f %f %f %f\n", poly->equation_rotz_only.v.x, poly->equation_rotz_only.v.y, poly->equation_rotz_only.v.z, poly->equation_rotz_only.d);
// 	// printf("Proj_ortho %f %f %f\n", proj_ortho.x, proj_ortho.y, proj_ortho.z);
// 	if (mag(proj_ortho) > player->width_2)
// 	{
// 		printf("Poly trop loin\n");
// 		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 		return (0);
// 	}
// 	if (is_in_poly(poly, proj_ortho))
// 	{
// 		printf("Sphere's dot is in the Poly\n");
// 		// printf("Eq : %f %f %f %f\n", poly->equation_rotz_only.v.x, poly->equation_rotz_only.v.y, poly->equation_rotz_only.v.z, poly->equation_rotz_only.d);
// 		// print_poly(poly, 1);
// 		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 		return (1);
// 	}
// 	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
// 	return (collision_segment(map, poly->dots_rotz_only, player->width_2) ||\
// 			collision_poly_dots_2(map, poly->dots_rotz_only, map->player.width_2));
// 	// return (collision_poly_dots(map, poly->dots_rotz_only, player->width_2) ||\
// 	// 		collision_segment(map, poly->dots_rotz_only, player->width_2));
// }

// t_poly			*collisions_sphere(t_map *map, t_player *player, t_poly *poly)
// {
// 	while (poly)
// 	{
// 		// print_poly(poly, 1);
// 		if (!poly->is_slide_ban)
// 		{
// 			if (collision_poly(map, player, poly))
// 			{
// 				// printf("COLLISION !!!\n");
// 				// printf("Is col : 1\n");
// 				return (poly);
// 			}
// 			// printf("Is col : 0\n");
// 		}
// 		poly = poly->next;
// 	}
// 	printf("\n\n");
// 	map = NULL;
// 	return (NULL);
// }