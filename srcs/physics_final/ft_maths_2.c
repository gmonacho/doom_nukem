#include "doom_nukem.h"

float				scalar_product(t_fdot_3d v1, t_fdot_3d v2)
{
	return ((float)(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z));
}

int					intersection_plan_line(t_fdot_3d *collision, t_plan plan, t_cartesienne *ray)
{
	float			t;
	float			denominateur;

	denominateur = plan.v.x * ray->vx + plan.v.y * ray->vy + plan.v.z * ray->vz;
	if (denominateur == 0)
	{
		printf("denom = 0\n");
		return (0);
	}
	printf("denom = %f\n", denominateur);
	t = -plan.d / (float)(denominateur);
	collision->x = ray->vx * t;
	collision->y = ray->vy * t;
	collision->z = ray->vz * t;
	return (1);
}
// int					intersection_plan_line(t_fdot_3d *collision, t_plan plan, t_cartesienne *ray)
// {
// 	float			t;
// 	float			denominateur;

// 	denominateur = plan.v.x * ray->vx + plan.v.y * ray->vy + plan.v.z * ray->vz;
// 	if (denominateur == 0)
// 		return (0);
// 	t = -(plan.v.x * ray->ox + plan.v.y * ray->oy + plan.v.z * ray->oz + plan.d) /\
// 			(float)(denominateur);
// 	collision->x = ray->vx * t + ray->ox;
// 	collision->y = ray->vy * t + ray->oy;
// 	collision->z = ray->vz * t + ray->oz;
// 	return (1);
// }

// float		prop(float value, t_dot inter1, t_dot inter2)
// {
// 	if (inter1.y == inter1.x)
// 		return ((inter2.y + inter2.x) / 2);
// 	return (inter2.x + ((value - inter1.x) / (float)(inter1.y - inter1.x)) *\
// 			(inter2.y - inter2.x));
// }

// float		fprop(float value, t_fdot inter1, t_fdot inter2)
// {
// 	if (inter1.y == inter1.x)
// 		return ((inter2.y + inter2.x) / 2);
// 	return (inter2.x + ((value - inter1.x) / (inter1.y - inter1.x)) *\
// 			(inter2.y - inter2.x));
// }

// float		fmag(t_fdot_3d dot)
// {
// 	return (sqrt(dot.x * dot.x + dot.y * dot.y + dot.z * dot.z));
// }

t_dot			intersection_segment_edge(t_win *win, t_dot d1, t_dot d2, int edge)
{
	float		a;
	float		b;
	float		denom;

	if (!(denom = d1.x - d2.x))
		return ((t_dot){d1.x, edge ? win->h - 1 : 0});
	a = (d1.y - d2.y) / denom;
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
