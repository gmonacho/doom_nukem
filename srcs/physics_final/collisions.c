#include "doom_nukem.h"

static int	is_in_poly(t_poly *poly, t_fdot_3d c1, t_fdot_3d c2)
{
	t_fdot	coord1;
	t_fdot	coord2;

	find_coord_plan(poly, &coord1, c1);
	find_coord_plan(poly, &coord2, c2);
	// print_poly(poly, 1);
	// printf("c1 c2 %f %f %f\t%f %f %f\n", c1.x, c1.y, c1.z, c2.x, c2.y, c2.z);
	// printf("Coord %f %f\t%f %f\n", coord1.x, coord1.y, coord2.x, coord2.y);
	// if ((!(coord1.x <= 0 || 1 <= coord1.x || coord1.y <= 0 || 1 <= coord1.y) ||\
	// 		!(coord2.x <= 0 || 1 <= coord2.x || coord2.y <= 0 || 1 <= coord2.y)) ? 0 : 1)
	// 	printf("coord exte !\n");
	return ((!(coord1.x <= 0 || 1 <= coord1.x || coord1.y <= 0 || 1 <= coord1.y) ||\
			!(coord2.x <= 0 || 1 <= coord2.x || coord2.y <= 0 || 1 <= coord2.y)) ? 1 : 0);
}

static int	resolve(t_poly *poly, t_fdot_3d polynome, float ad, float bd, int zc)
{
	t_fdot_3d	c1;
	t_fdot_3d	c2;

	if (!is_null(poly->equation_rotz_only.v.x, 0.0005))
	{
		// c1.y = (-polynome.y - sqrt(delta)) / (2 * polynome.x);
		// c2.y = (-polynome.y + sqrt(delta)) / (2 * polynome.x);
		if (!resolve_polynome(polynome, &(c1.y), &(c2.y)))
			return (0);
		c1.x = ad * c1.y + bd;
		c2.x = ad * c2.y + bd;
	}
	else
	{
		// c1.x = (-polynome.y - sqrt(delta)) / (2 * polynome.x);
		// c2.x = (-polynome.y + sqrt(delta)) / (2 * polynome.x);
		if (!resolve_polynome(polynome, &(c1.x), &(c2.x)))
			return (0);
		c1.y = ad * c1.x + bd;
		c2.y = ad * c2.x + bd;
	}
	c1.z = zc;
	c2.z = zc;
	return (is_in_poly(poly, fdot_3d_sub(c1, poly->dots[0]), fdot_3d_sub(c2, poly->dots[0])));
}

static int	collision_circle(t_player *player, t_poly *poly, int zc, int radius)
{
	t_fdot_3d	polynome;
	float		ad;
	float		bd;

	int			ret1 = 0;
	int			ret2 = 0;

	if (!is_null(poly->equation_rotz_only.v.x, 0.0005))
	{
		ad = -poly->equation_rotz_only.v.y / poly->equation_rotz_only.v.x;
		bd = -(poly->equation_rotz_only.v.z * zc + poly->equation_rotz_only.d) / poly->equation_rotz_only.v.x;
		// printf("collision div v.x\n");
	}
	else if (!is_null(poly->equation_rotz_only.v.y, 0.0005))
	{
		ad = -poly->equation_rotz_only.v.x / poly->equation_rotz_only.v.y;
		bd = -(poly->equation_rotz_only.v.z * zc + poly->equation_rotz_only.d) / poly->equation_rotz_only.v.y;
		// printf("collision div v.y\n");
	}
	else
	{
		// if (zc == -player->height / 2)
		// 	print_poly(poly, 1);
		if (!(poly->dots_rotz_only[0].z < -player->_9_height_10 || player->height_10 < poly->dots_rotz_only[0].z) &&\
			((ret1 = is_in_poly(poly, fdot_3d_sub((t_fdot_3d){0, 0, poly->dots_rotz_only[0].z}, poly->dots_rotz_only[0]),\
							fdot_3d_sub((t_fdot_3d){0, 0, poly->dots_rotz_only[0].z}, poly->dots_rotz_only[0]))) ||\
			(ret2 = is_intersection_cercle_poly(poly, radius))))
		{
			// printf("is in poly %d\tis intersec %d\n", ret1, ret2);
			if (poly->dots_rotz_only[0].z == -player->_9_height_10)
			{
				player->on_floor = 1;
				// printf("Floor !\n");
			}
			return (1);
		}
		else
		{
			// if (zc == -player->height / 2)
				// printf("Col 0\n");
			// printf("");
			if (poly->dots[0].z == -player->_9_height_10)
				player->on_floor = 0;
			return (0);
		}
		
		// return (!(delta < -player->height || 0 < delta) &&\
		// 		is_in_poly(poly, fdot_3d_sub((t_fdot_3d){0, 0, delta}, poly->dots_rotz_only[0]),\
		// 						fdot_3d_sub((t_fdot_3d){0, 0, delta}, poly->dots_rotz_only[0])) ? 1 : 0);
	}	
	polynome.x = 1 + ad * ad;
	polynome.y = 2 * ad * bd;
	polynome.z = bd * bd - radius * radius;

	// if ((delta = polynome.y * polynome.y - 4 * polynome.x * polynome.z) < 0)
	// 	return (0);
	return (resolve(poly, polynome, ad, bd, zc));
}

int			is_poly_collision(t_player *player, t_poly *poly)
{
	int		z;

	z = -player->_9_height_10;
	while (z++ < player->height_10)
	{
		if (collision_circle(player, poly, z, player->width / 2))
		{
			// printf("Col %f %f %f %f\n", poly->equation_rotz_only.v.x, poly->equation_rotz_only.v.y, poly->equation_rotz_only.v.z, poly->equation_rotz_only.d);
			return (1);
		}
	}
	return (0);
}

t_poly		*collisions(t_player *player, t_poly *poly)
{
	while (poly)
	{
		if (!poly->is_slide_ban)
		{
			if (is_poly_collision(player, poly))
				return (poly);
		}
		poly = poly->next;
	}
	return (NULL);
}