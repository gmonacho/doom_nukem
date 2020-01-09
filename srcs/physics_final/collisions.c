#include "doom_nukem.h"

static int	is_in_poly(t_poly *poly, t_fdot_3d c1, t_fdot_3d c2)
{
	t_fdot	coord1;
	t_fdot	coord2;

	find_coord_plan(poly, &coord1, c1);
	find_coord_plan(poly, &coord2, c2);
	return ((!(coord1.x <= 0 || 1 <= coord1.x || coord1.y <= 0 || 1 <= coord1.y) ||\
			!(coord2.x <= 0 || 1 <= coord2.x || coord2.y <= 0 || 1 <= coord2.y)) ? 1 : 0);
}

static int	resolve(t_poly *poly, t_fdot_3d polynome, float delta, int zc)
{
	t_fdot_3d	c1;
	t_fdot_3d	c2;

	if (!is_null(poly->equation_rotz_only.v.x, 0.0005))
	{
		c1.x = (-polynome.y - sqrt(delta)) / (2 * polynome.x);
		c1.y = polynome.x * c1.x + polynome.y;
		c2.x = (-polynome.y + sqrt(delta)) / (2 * polynome.x);
		c2.y = polynome.x * c1.x + polynome.y;
	}
	else
	{
		c1.y = (-polynome.y - sqrt(delta)) / (2 * polynome.x);
		c1.x = polynome.x * c1.y + polynome.y;
		c2.y = (-polynome.y + sqrt(delta)) / (2 * polynome.x);
		c2.x = polynome.x * c1.y + polynome.y;
	}
	c1.z = zc;
	c2.z = zc;
	return (is_in_poly(poly, c1, c2));
}

static int	collision_circle(t_player *player, t_poly *poly, int zc, int width)
{
	t_fdot_3d	polynome;
	float		delta;
	float		ad;
	float		bd;

	if (!is_null(poly->equation_rotz_only.v.x, 0.0005))
	{
		ad = -poly->equation_rotz_only.v.y / poly->equation_rotz_only.v.x;
		bd = -(poly->equation_rotz_only.v.z * zc + poly->equation_rotz_only.d) / poly->equation_rotz_only.v.x;
	}
	else if (!is_null(poly->equation_rotz_only.v.y, 0.0005))
	{
		ad = -poly->equation_rotz_only.v.x / poly->equation_rotz_only.v.y;
		bd = -(poly->equation_rotz_only.v.z * zc + poly->equation_rotz_only.d) / poly->equation_rotz_only.v.y;
	}
	else
	{
		delta = -poly->equation_rotz_only.d / poly->equation_rotz_only.v.z;
		return ((delta < -player->height || 0 < delta) ? 0 : 1);
	}
	polynome.x = 1 + ad * ad;
	polynome.y = 2 * ad * bd;
	polynome.z = bd * bd - width * width;
	if ((delta = polynome.y * polynome.y - 4 * polynome.x * polynome.z) < 0)
		return (0);
	else
		return (resolve(poly, polynome, delta, zc));
}

int		collisions(t_player *player, t_poly *poly)
{
	int		z;
	// int		ret;

	while (poly)
	{
		z = -player->height;
		while (z++ < 0)
		{
			if (collision_circle(player, poly, z, player->width))
				return (1);
			// ret = is_collision(player, poly, z, player->width);
			// if (ret)
			// 	return (1);
			// 	printf("Col cercle %d : %d\n", z, ret);
		}
		// if (ret)
		// 	print_poly(poly, 1);
		poly = poly->next;
	}
	return (0);
	//printf("\n\n");
}