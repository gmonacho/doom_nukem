#include "doom_nukem.h"

t_fdot_3d			proj_ortho_origin_line(t_fdot_3d d1, t_fdot_3d d2, t_fdot_3d *proj)
{
	float			prop;
	t_fdot_3d		line;

	line = (t_fdot_3d){d2.x - d1.x, d2.y - d1.y, d2.z - d1.z};
	prop = scalar_product((t_fdot_3d){-d1.x, -d1.y, -d1.z},\
							line) /\
			scalar_product(line, line);
	proj->x = d1.x + line.x * prop;
	proj->y = d1.y + line.y * prop;
	proj->z = d1.z + line.z * prop;
	return ((t_fdot_3d){proj->x, proj->y, proj->z});
}

void				proj_ortho_plan(t_fdot_3d dot, t_plan plan, t_fdot_3d *proj_ortho)
{
	intersection_plan_ray(proj_ortho, plan,\
					(t_cartesienne){dot.x, dot.y, dot.z, plan.v.x, plan.v.y, plan.v.z,\
									0, NULL, 0, (t_fdot_3d){}, NULL});
}

int					intersection_plan_my_ray(t_fdot_3d *collision, t_plan plan, t_cartesienne *ray)
{
	float			t;
	float			denominateur;

	denominateur = plan.v.x + plan.v.y * ray->vy + plan.v.z * ray->vz;
	if (denominateur == 0)
		return (0);
	t = -plan.d / denominateur;
	collision->x = t;
	collision->y = ray->vy * t;
	collision->z = ray->vz * t;
	return (1);
}

int					intersection_plan_ray(t_fdot_3d *collision, t_plan plan, t_cartesienne ray)
{
	float			t;
	float			denominateur;

	denominateur = plan.v.x * ray.vx + plan.v.y * ray.vy + plan.v.z * ray.vz;
	if (is_null(denominateur, 0.005))
		return (0);
	t = (-plan.d - plan.v.x * ray.ox - plan.v.y * ray.oy - plan.v.z * ray.oz) / denominateur;
	collision->x = ray.vx * t + ray.ox;
	collision->y = ray.vy * t + ray.oy;
	collision->z = ray.vz * t + ray.oz;
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

/*
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
	if (edge == 0)
		return ((t_dot){-b / a, 0});
	else if (edge == 1)
		return ((t_dot){win->w - 1, a * (win->w - 1) + b});
	else if (edge == 2)
		return ((t_dot){(win->h - 1 - b) / a, win->h - 1});
	else
		return ((t_dot){0, b});
}
*/