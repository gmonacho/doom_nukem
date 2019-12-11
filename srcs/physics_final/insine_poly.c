#include "doom_nukem.h"

static int	is_intersection(t_dot d1, t_dot d2, int x, int y)
{
	double	a;
	double	b;
	int		num;
	int		denom;
	double	x2;

	if (!(num = d1.y - d2.y))
		return (d1.y == y);
	if (!(denom = d1.x - d2.x))
		return (0 < d1.x && d1.x < x &&\
				((d1.y < y && y < d2.y) ||\
				(d2.y < y && y < d1.y)));
	a = num / (double)denom;
	b = d1.y - a * d1.x;
	x2 = (y - b) / a;
	return (!(x2 < 0 || x2 > x));
}

static int	inside_one_poly(t_poly *poly, int x, int y)
{
	int		i;
	int		i2;
	int		count;

	count = 0;
	i = -1;
	while (++i < poly->n_proj)
	{
		i2 = i ? i - 1 : poly->n_proj - 1;
		if (is_intersection(poly->dots_proj[i], poly->dots_proj[i2], x, y))
		{
			printf("d1 %d %d\td2 %d %d\t%d\n", poly->dots_proj[i].x, poly->dots_proj[i].y, poly->dots_proj[i2].x, poly->dots_proj[i2].y, 1);
			count++;
		}
		else
			printf("d1 %d %d\td2 %d %d\t%d\n", poly->dots_proj[i].x, poly->dots_proj[i].y, poly->dots_proj[i2].x, poly->dots_proj[i2].y, 0);
	}
	printf("%d %d : n edge %d with poly %p nproj %d\n", x, y, count, poly, poly->n_proj);
	return (count % 2);
}

t_poly		*inside_poly(t_poly *last_poly, t_poly *poly, int x, int y)
{
	if (last_poly && inside_one_poly(last_poly, x, y))
		return (last_poly);
	while (poly)
	{
		if (poly != last_poly &&\
			inside_one_poly(poly, x, y))
			return (poly);
		poly = poly->next;
	}
	printf("A linterieur de personne wtfff ?\n");
	exit(0);
	return (NULL);
}