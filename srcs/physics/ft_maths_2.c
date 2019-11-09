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

int			intersection_plan_line_static(t_fdot_3d *collision, t_plan plan, t_cartesienne *ray)
{
	double			t;
	
	if (plan.a * ray->vx + plan.b * ray->vy + plan.c * ray->vz == 0)
		return (0);
	t = -(plan.a * ray->ox + plan.b * ray->oy + plan.c * ray->oz + plan.d) /\
			(double)(plan.a * ray->vx + plan.b * ray->vy + plan.c * ray->vz);
	collision->x = ray->vx * t + ray->ox;
	collision->y = ray->vy * t + ray->oy;
	collision->z = ray->vz * t + ray->oz;
	return (1);
}

// void		draw_affine(t_win *win, t_affine function)
// {
// 	SDL_SetRenderDrawColor(win->rend, 0, 0, 0, 175);
// 	draw_line(	win,\
// 				(t_dot){0,		function.b},\
// 				(t_dot){win->w,	function.a * win->w + function.b});
// }

// void		draw_ray(t_win *win, t_player *player, t_affine ray)
// {
// 	SDL_SetRenderDrawColor(win->rend, 0, 0, 0, 175);
// 	// printf("Ray angle : %f\n", ray.angle);
// 	if (ray.angle < M_PI_2 || ray.angle > 3 * M_PI_2)
// 		draw_line(	win,\
// 					(t_dot){player->pos.x,	ray.a * player->pos.x + ray.b},\
// 					(t_dot){win->w,			ray.a * win->w + ray.b});
// 	else
// 		draw_line(	win,\
// 					(t_dot){0,				ray.b},\
// 					(t_dot){player->pos.x,	ray.a * player->pos.x + ray.b});
// }

// int			lines_intersection(t_fdot *intersection, t_affine *line1, t_affine *line2)
// {
// 	// printf("Is : %d\n", line1->isequation);
// 	if (line1->isequation)
// 	{
// 		if (line2->isequation)
// 			intersection->x = (line1->b - line2->b) /\
// 						(double)(line2->a - line1->a);
// 		else
// 			intersection->x = line2->a;
// 		intersection->y = line1->a * intersection->x + line1->b;
// 		// printf("Diff a = %f\tDiff b = %f\tIntersectino x = %f\n", line2->a - line1->a, line1->b - line2->b, (line1->b - line2->b) /\
// 						// (double)(line2->a - line1->a));
// 	}
// 	else
// 	{
// 		if (line2->isequation)
// 			intersection->x = line1->a;
// 		else
// 			return (0);
// 		intersection->y = line2->a * intersection->x + line2->b;
// 	}
// 	return (1);
// }
