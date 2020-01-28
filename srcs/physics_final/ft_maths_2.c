#include "doom_nukem.h"

// float				dist_origin_line(t_fdot_3d l1, t_fdot_3d l2)
// {
// 	return (mag(vectoriel_product((t_fdot_3d){-l1.x, -l1.y, -l1.z},\
// 							(t_fdot_3d){l2.x - l1.x,\
// 										l2.y - l1.y,\
// 										l2.z - l1.z})) /\
// 			fdist_3d(l1, l2));
// }

t_fdot_3d			proj_ortho_origin_line(t_fdot_3d l1, t_fdot_3d l2, t_fdot_3d *proj)
{
	float			prop;
	t_fdot_3d		line;

	line = (t_fdot_3d){l2.x - l1.x, l2.y - l1.y, l2.z - l1.z};
	prop = scalar_product((t_fdot_3d){-l1.x, -l1.y, -l1.z},\
							line) /\
			scalar_product(line, line);
	proj->x = l1.x + line.x * prop;
	proj->y = l1.y + line.y * prop;
	proj->z = l1.z + line.z * prop;
	return ((t_fdot_3d){proj->x, proj->y, proj->z});
	// return ((t_fdot_3d){l1.x + line.x * prop, l1.y + line.y * prop, l1.z + line.z * prop});
}

void				proj_ortho_plan(t_fdot_3d dot, t_plan plan, t_fdot_3d *proj_ortho)
{
	intersection_plan_ray(proj_ortho, plan,\
					(t_cartesienne){dot.x, dot.y, dot.z, plan.v.x, plan.v.y, plan.v.z,\
									NULL, 0, 0, NULL});
}

int					intersection_plan_my_ray(t_fdot_3d *collision, t_plan plan, t_cartesienne *ray)
{
	float			t;
	float			denominateur;

	denominateur = plan.v.x + plan.v.y * ray->vy + plan.v.z * ray->vz;
	// denominateur = plan.v.x * ray->vx + plan.v.y * ray->vy + plan.v.z * ray->vz;
	if (denominateur == 0)
	{
		// printf("denom = 0\n");
		return (0);
	}
	// printf("denom = %f\n", denominateur);
	t = -plan.d / denominateur;
	collision->x = t;
	collision->y = ray->vy * t;
	collision->z = ray->vz * t;
	if (collision->x != collision->x || ray->vy != ray->vy || t != t)
	{
		printf("Collision %f %f %f\n", collision->x, collision->y, collision->z);
		printf("Plan %f %f %f\n", plan.v.x, plan.v.y, plan.v.z);
		printf("Ray %f %f %f\n", ray->vx, ray->vy, ray->vz);
		printf("t %f\n", t);
		exit(0);
	}
	return (1);
}

int					intersection_plan_ray(t_fdot_3d *collision, t_plan plan, t_cartesienne ray)
{
	float			t;
	float			denominateur;

	denominateur = plan.v.x * ray.vx + plan.v.y * ray.vy + plan.v.z * ray.vz;
	// printf("Dpos o %f %f %f\n", ray.ox, ray.oy, ray.oz);
	// printf("Dpos v %f %f %f\n", ray.vx, ray.vy, ray.vz);
	// printf("Plan %f %f %f %f\n", plan.v.x, plan.v.y, plan.v.z, plan.d);
	// printf("Proj ortho %f %f %f\n\n", collision->x, collision->y, collision->z);
	// printf("%f\n", denominateur);
	if (is_null(denominateur, 0.005))
		return (0);
	t = (-plan.d - plan.v.x * ray.ox - plan.v.y * ray.oy - plan.v.z * ray.oz) / denominateur;
	collision->x = ray.vx * t + ray.ox;
	collision->y = ray.vy * t + ray.oy;
	collision->z = ray.vz * t + ray.oz;
	// printf("Proj ortho %f %f %f\n\n", collision->x, collision->y, collision->z);
	return (1);
}

int				resolve_polynome(t_fdot_3d polynome, float *x1, float *x2)
{
	float		delta;

	if ((delta = polynome.y * polynome.y - 4 * polynome.x * polynome.z) < 0)
		return (0);
	*x1 = (-polynome.y - sqrt(delta)) / (2 * polynome.x);
	*x2 = (-polynome.y + sqrt(delta)) / (2 * polynome.x);
	return (1);
}

static int		is_intersection_cercle_segment(t_fdot_3d d1, t_fdot_3d d2, int radius)
{
	t_fdot_3d	polynome;
	t_fdot_3d	c1;
	t_fdot_3d	c2;
	float		a;
	float		b;
	float		denom;
	
	if (is_null(denom = d2.x - d1.x, 0.005))
	{
		if ((denom = radius * radius - d1.x * d1.x) >= 0)
		{
			denom = sqrt(denom);
			if (!((denom < d1.y && denom < d2.y) ||\
				(denom > d1.y && denom > d2.y)))
				return (1);
			// printf("Denom null\n");
			// printf("D1 %f %f %f\n", d1.x, d1.y, d1.z);
			// printf("D2 %f %f %f\n", d2.x, d2.y, d2.z);
		}
		return (0);
	}
	a = (d2.y - d1.y) / denom;
	b = d1.y - a * d1.x;
	// printf("ab %f %f\n", a, b);
	polynome.x = 1 + a * a;
	polynome.y = 2 * a * b;
	polynome.z = b * b - radius * radius;
	if (!resolve_polynome(polynome, &(c1.x), &(c2.x)))
		return (0);
	c1.y = a * c1.x + b;
	c2.y = a * c2.x + b;
	if (((c1.x < d1.x && c1.x < d2.x) ||\
		(c1.x > d1.x && c1.x > d2.x) ||\
		(c1.y < d1.y && c1.y < d2.y) ||\
		(c1.y > d1.y && c1.y > d2.y)) &&\
		((c2.x < d1.x && c2.x < d2.x) ||\
		(c2.x > d1.x && c2.x > d2.x) ||\
		(c2.y < d1.y && c2.y < d2.y) ||\
		(c2.y > d1.y && c2.y > d2.y)))
		return (0);
	// printf("Cas general\n");
	// printf("Denom %f\n", denom);
	// printf("D : %fx + %f\n", a, b);
	// printf("C1 %f %f %f\n", c1.x, c1.y, c1.z);
	// printf("C2 %f %f %f\n", c2.x, c2.y, c2.z);
	// printf("D1 %f %f %f\n", d1.x, d1.y, d1.z);
	// printf("D2 %f %f %f\n", d2.x, d2.y, d2.z);
	return (1);
}

int				is_intersection_cercle_poly(t_poly *poly, int radius)
{
	int			i;

	// printf("Sol 1\n");
	i = -1;
	while (++i < N_DOTS_POLY)
	{
		if (is_intersection_cercle_segment(poly->dots_rotz_only[i], poly->dots_rotz_only[(i ? i : N_DOTS_POLY) - 1], radius))
		{
			// printf("i : %d %d\n", i, (i ? i : N_DOTS_POLY) - 1);
			return (1);
		}
	}
	// printf("Sol 0 f\n");
	return (0);
}

t_dot			intersection_segment_edge(t_win *win, t_dot d1, t_dot d2, int edge)
{
	float		a;
	float		b;
	float		denom;

	if (!(denom = d1.x - d2.x))
		return ((t_dot){d1.x, edge ? win->h - 1 : 0});
	if (!(a = (d1.y - d2.y) / denom))
		return ((t_dot){edge == 1 ? win->w - 1 : 0, d1.y});
	b = d1.y - a * d1.x;
	// printf("Line inter screen : y = %f * x + %f\n", a, b);
	if (edge == 0)
	{
		// printf("Collision haut %d %d", collision->x, collision->y);
		return ((t_dot){-b / a, 0});
	}
	else if (edge == 1)
	{
		// printf("Collision droite %d %d / %d %d / %d %d", d1.x, d1.y, collision->x, collision->y, d2.x, d2.y);
		return ((t_dot){win->w - 1, a * (win->w - 1) + b});
	}
	else if (edge == 2)
	{
		// printf("Collision bas %d %d", collision->x, collision->y);
		return ((t_dot){(win->h - 1 - b) / a, win->h - 1});
	}
	else
	{
		// printf("Collision gauche %d %d", collision->x, collision->y);
		return ((t_dot){0, b});
	}
}
