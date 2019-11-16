#include "doom_nukem.h"

double		prop(double value, t_dot inter1, t_dot inter2)
{
	if (inter1.y == inter1.x)
		return ((inter2.y + inter2.x) / 2);
	return (inter2.x + ((value - inter1.x) / (double)(inter1.y - inter1.x)) *\
			(inter2.y - inter2.x));
}

double		fprop(double value, t_fdot inter1, t_fdot inter2)
{
	if (inter1.y == inter1.x)
		return ((inter2.y + inter2.x) / 2);
	return (inter2.x + ((value - inter1.x) / (inter1.y - inter1.x)) *\
			(inter2.y - inter2.x));
}

double		fmag(t_fdot_3d dot)
{
	return (sqrt(dot.x * dot.x + dot.y * dot.y + dot.z * dot.z));
}

int			intersection_plan_line(t_fdot_3d *collision, t_plan plan, t_cartesienne *ray)
{
	double			t;

	if (plan.v.x * ray->vx + plan.v.y * ray->vy + plan.v.z * ray->vz == 0)
		return (0);
	t = -(plan.v.x * ray->ox + plan.v.y * ray->oy + plan.v.z * ray->oz + plan.d) /\
			(double)(plan.v.x * ray->vx + plan.v.y * ray->vy + plan.v.z * ray->vz);
	collision->x = ray->vx * t + ray->ox;
	collision->y = ray->vy * t + ray->oy;
	collision->z = ray->vz * t + ray->oz;
	return (1);
}

int			lines_intersection(t_fdot *intersection, t_affine *line1, t_affine *line2)
{
	if (line1->isequation)
	{
		if (line2->isequation)
			intersection->x = (line1->b - line2->b) /\
						(double)(line2->a - line1->a);
		else
			intersection->x = line2->a;
		intersection->y = line1->a * intersection->x + line1->b;
	}
	else
	{
		if (line2->isequation)
			intersection->x = line1->a;
		else
			return (0);
		intersection->y = line2->a * intersection->x + line2->b;
	}
	return (1);
}
