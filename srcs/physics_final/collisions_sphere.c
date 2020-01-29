#include "doom_nukem.h"

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

int				collision_dots(t_map *map, t_fdot_3d dots[N_DOTS_POLY], float ray)
{
	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, map->player._4_height_10});
	if (mag(dots[0]) <= ray ||\
		mag(dots[1]) <= ray ||\
		mag(dots[2]) <= ray ||\
		mag(dots[3]) <= ray)
	{
		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
		return (1);
	}
	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
	return (0);
}

int				collision_segment(t_map *map, t_fdot_3d dots[4], float width_2)
{
	t_fdot_3d	proj_ortho[N_DOTS_POLY];

	// printf("Dot 0 : %f %f %f\n", dots[0].x, dots[0].y, dots[0].z);
	// printf("Dot 1 : %f %f %f\n", dots[1].x, dots[1].y, dots[1].z);
	// printf("Dot 2 : %f %f %f\n", dots[2].x, dots[2].y, dots[2].z);
	// printf("Dot 3 : %f %f %f\n", dots[3].x, dots[3].y, dots[3].z);

	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, map->player._4_height_10});
	if (is_in_segment(proj_ortho_origin_line(dots[0], dots[1], proj_ortho),\
						dots[0], dots[1]) &&\
		mag(proj_ortho[0]) <= width_2)
	{
		printf("Poly's segment is in the Sphere, code 0b0001\n");
		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
		return (0b0001);
	}
	if (is_in_segment(proj_ortho_origin_line(dots[1], dots[2], proj_ortho + 1),\
						dots[1], dots[2]) &&\
		mag(proj_ortho[1]) <= width_2)
	{
		printf("Poly's segment is in the Sphere, code 0b0110\n");
		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
		return (0b0110);
	}
	if (is_in_segment(proj_ortho_origin_line(dots[2], dots[3], proj_ortho + 2),\
						dots[2], dots[3]) &&\
		mag(proj_ortho[2]) <= width_2)
	{
		printf("Poly's segment is in the Sphere, code 0b1011\n");
		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
		return (0b1011);
	}
	if (is_in_segment(proj_ortho_origin_line(dots[3], dots[0], proj_ortho + 3),\
						dots[3], dots[0]) &&\
		mag(proj_ortho[3]) <= width_2)
	{
		printf("Poly's segment is in the Sphere, code 0b1100\n");
		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
		return (0b1100);
	}
	printf("Poly is not in the Sphere\n");
	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
	return (0);
}


int				collision_poly(t_map *map, t_player *player, t_poly *poly)
{
	t_fdot_3d	proj_ortho;

	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, map->player._4_height_10});
	proj_ortho_plan((t_fdot_3d){0, 0, 0}, poly->equation_rotz_only, &proj_ortho);
	// printf("Plan %f %f %f %f\n", poly->equation_rotz_only.v.x, poly->equation_rotz_only.v.y, poly->equation_rotz_only.v.z, poly->equation_rotz_only.d);
	// printf("Proj_ortho %f %f %f\n", proj_ortho.x, proj_ortho.y, proj_ortho.z);
	if (mag(proj_ortho) > player->width_2)
	{
		printf("Poly trop loin\n");
		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
		return (0);
	}
	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
	if ((poly->segment_code = collision_segment(map, poly->dots_rotz_only, player->width_2)) ||\
		collision_dots(map, poly->dots_rotz_only, map->player.width_2))
		return (1);
	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, map->player._4_height_10});
	if (is_in_poly(poly, proj_ortho))
	{
		printf("Proj ortho is in the Poly\n");
		printf("Proj ortho %f %f %f\n", proj_ortho.x, proj_ortho.y, proj_ortho.z);
		// printf("Eq : %f %f %f %f\n", poly->equation_rotz_only.v.x, poly->equation_rotz_only.v.y, poly->equation_rotz_only.v.z, poly->equation_rotz_only.d);
		// print_poly(poly, 1);
		translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
		return (1);
	}
	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
	return (0);
	// return ((poly->segment_code = collision_segment(map, poly->dots_rotz_only, player->width_2)) ||\
	// 		collision_dots(map, poly->dots_rotz_only, map->player.width_2));
}

t_poly			*collisions_sphere(t_map *map, t_player *player, t_poly *poly)
{
	while (poly)
	{
		// print_poly(poly, 1);
		if (collision_poly(map, player, poly))
		{
			// printf("COLLISION !!!\n");
			return (poly);
		}
		poly = poly->next;
	}
	printf("\n\n");
	return (NULL);
}