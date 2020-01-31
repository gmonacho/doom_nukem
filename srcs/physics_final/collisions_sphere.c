#include "doom_nukem.h"

static int		is_in_poly_rotz_only(t_poly *poly, t_fdot_3d dot)
{
	t_fdot_3d	dot_3d_in_plan;
	t_fdot		coord1;

	dot_3d_in_plan = fdot_3d_sub(dot, poly->dots_rotz_only[0]);
	coord1.x = scalar_product(dot_3d_in_plan, poly->i_rotz_only) / poly->ii;
	coord1.y = scalar_product(dot_3d_in_plan, poly->j_rotz_only) / poly->jj;
	return (coord1.x < 0 || 1 < coord1.x || coord1.y < 0 || 1 < coord1.y ? 0 : 1);
}

static int		is_in_segment(t_fdot_3d is, t_fdot_3d d1, t_fdot_3d d2)
{
	if (!is_null(d2.x - d1.x, 0.0005))
	{
		if (!(is.x > d1.x || is.x > d2.x) ||\
			!(is.x < d1.x || is.x < d2.x))
			return (0);
		return (1);
	}
	else if (!is_null(d2.y - d1.y, 0.0005))
	{
		if (!(is.y > d1.y || is.y > d2.y) ||\
			!(is.y < d1.y || is.y < d2.y))
			return (0);
		return (1);
	}
	else
	{
		if (!(is.z > d1.z || is.z > d2.z) ||\
			!(is.z < d1.z || is.z < d2.z))
			return (0);
		return (1);
	}
}

int				collision_dots(t_map *map, t_fdot_3d dots[N_DOTS_POLY], float ray)
{
	if (mag(dots[0]) <= ray ||\
		mag(dots[1]) <= ray ||\
		mag(dots[2]) <= ray ||\
		mag(dots[3]) <= ray)
	{
		return (1);
	}
	map = NULL;
	return (0);
}

int				collision_segment(t_map *map, t_fdot_3d dots[4], float width_2)
{
	t_fdot_3d	proj_ortho;

	if (is_in_segment(proj_ortho_origin_line(dots[0], dots[1], &proj_ortho),\
						dots[0], dots[1]) &&\
		mag(proj_ortho) <= width_2)
	{
		// printf("Poly's segment is in the Sphere, code 0b0001\n");
		return (0b0001);
	}
	if (is_in_segment(proj_ortho_origin_line(dots[1], dots[2], &proj_ortho),\
						dots[1], dots[2]) &&\
		mag(proj_ortho) <= width_2)
	{
		// printf("Poly's segment is in the Sphere, code 0b0110\n");
		return (0b0110);
	}
	// printf("Proj ortho segment %f %f %f\tMag : %f\n", proj_ortho.x, proj_ortho.y, proj_ortho.z, mag(proj_ortho));
	if (is_in_segment(proj_ortho_origin_line(dots[2], dots[3], &proj_ortho),\
						dots[2], dots[3]) &&\
		mag(proj_ortho) <= width_2)
	{
		// printf("Poly's segment is in the Sphere, code 0b1011\n");
		return (0b1011);
	}
	if (is_in_segment(proj_ortho_origin_line(dots[3], dots[0], &proj_ortho),\
						dots[3], dots[0]) &&\
		mag(proj_ortho) <= width_2)
	{
		// printf("Poly's segment is in the Sphere, code 0b1100\n");
		return (0b1100);
	}
	map = NULL;
	return (0);
}



int				collision_poly(t_map *map, t_player *player, t_poly *poly)
{
	t_fdot_3d	proj_ortho;

	proj_ortho_plan((t_fdot_3d){0, 0, 0}, poly->equation_rotz_only, &proj_ortho);
	if (mag(proj_ortho) > player->width_2)
	{
		// printf("Poly trop loin\n");
		return (0);
	}
	if (is_in_poly_rotz_only(poly, proj_ortho))
	{
		return (1);
	}

	if ((poly->segment_code = collision_segment(map, poly->dots_rotz_only, player->width_2)) ||\
		collision_dots(map, poly->dots_rotz_only, map->player.width_2))
		return (1);
	// printf("Poly is not in the Sphere\n");
	return (0);
}

t_poly			*collisions_sphere(t_map *map, t_player *player, t_poly *poly, int ban_interest)
{
	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, map->player._4_height_10});
	while (poly)
	{
		// print_poly(poly, 1);
		if ((!ban_interest || !poly->is_slide_ban) &&\
			collision_poly(map, player, poly))
		{
			// printf("COLLISION !!! Poly %d is ban : %d\n", poly->index, poly->is_slide_ban);
			translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
			return (poly);
		}
		poly = poly->next;
	}
	translate_all_rotz_only(map->polys, (t_fdot_3d){0, 0, -map->player._4_height_10});
	// printf("\n\n");
	return (NULL);
}