#include "doom_nukem.h"

//Norme : creer rotate_poly()

void				rotate_box(t_map *map, t_player *player, t_object *object)
{
	t_poly			*poly;
	t_matrix		matrix;
	// t_fdot_3d		translate;
	int				i;

	matrix = create_ry_matrix(-object->rot_y_save + player->rot_y);
	object->pos = rotate_dot(object->pos, matrix);
	// translate = (t_fdot_3d){object->pos.x / 4, object->pos.y / 4, object->pos.z / 4};
	poly = object->poly;
	i = -1;
	while (++i < 6)
	{
		poly->equation_rotz_only.v = rotate_dot(poly->equation_rotz_only.v, matrix);

		poly->dots_rotz_only[0] = rotate_dot(poly->dots_rotz_only[0], matrix);
		poly->dots_rotz_only[1] = rotate_dot(poly->dots_rotz_only[1], matrix);
		poly->dots_rotz_only[2] = rotate_dot(poly->dots_rotz_only[2], matrix);
		poly->dots_rotz_only[3] = rotate_dot(poly->dots_rotz_only[3], matrix);

		// translate_dot(&(poly->dots_rotz_only[0]), translate);
		// translate_dot(&(poly->dots_rotz_only[1]), translate);
		// translate_dot(&(poly->dots_rotz_only[2]), translate);
		// translate_dot(&(poly->dots_rotz_only[3]), translate);

		poly->i_rotz_only = (t_fdot_3d){poly->dots_rotz_only[1].x - poly->dots_rotz_only[0].x,\
										poly->dots_rotz_only[1].y - poly->dots_rotz_only[0].y,\
										poly->dots_rotz_only[1].z - poly->dots_rotz_only[0].z};
		poly->j_rotz_only = (t_fdot_3d){poly->dots_rotz_only[N_DOTS_POLY - 1].x - poly->dots_rotz_only[0].x,\
										poly->dots_rotz_only[N_DOTS_POLY - 1].y - poly->dots_rotz_only[0].y,\
										poly->dots_rotz_only[N_DOTS_POLY - 1].z - poly->dots_rotz_only[0].z};
		poly = poly->next;
	}
	map = NULL;
}

t_fdot_3d			rotate_dot(t_fdot_3d dot, t_matrix matrix)
{
	return ((t_fdot_3d){dot.x * matrix._00 + dot.y * matrix._10 + dot.z * matrix._20,\
						dot.x * matrix._01 + dot.y * matrix._11 + dot.z * matrix._21,\
						dot.x * matrix._02 + dot.y * matrix._12 + dot.z * matrix._22});
}

void				translate_dot(t_fdot_3d *dot, t_fdot_3d translation)
{
	// printf("\t\tDot %f %f %f\n", dot->x, dot->y, dot->z);
	dot->x += translation.x;
	dot->y += translation.y;
	dot->z += translation.z;
	// printf("\t\tDot %f %f %f\n", dot->x, dot->y, dot->z);
}

// t_fdot_3d		rotate_dot(t_fdot_3d *dot, t_matrix matrix)
// {
// 	float			save_vx;
// 	float			save_vy;

// 	save_vx = dot->x * matrix._00 + dot->y * matrix._10 + dot->z * matrix._20;
// 	save_vy = dot->x * matrix._01 + dot->y * matrix._11 + dot->z * matrix._21;
// 	dot->z = dot->x * matrix._02 + dot->y * matrix._12 + dot->z * matrix._22;
// 	dot->x = save_vx;
// 	dot->y = save_vy;
//	return (*dot);
// }


void				rotate_all_rotz_only(t_map *map, t_poly *poly, t_matrix matrix)
{
	t_object		*object;

	while (poly)
	{
		if (poly->object && poly->object->type == BOX && poly->object->visible == 0)
		{
			poly = poly->next;
			continue ;	//Garde l'objet devant soi pour garder l'orientation et la distance
		}
		poly->equation_rotz_only.v = rotate_dot(poly->equation_rotz_only.v, matrix);

		poly->dots_rotz_only[0] = rotate_dot(poly->dots_rotz_only[0], matrix);
		poly->dots_rotz_only[1] = rotate_dot(poly->dots_rotz_only[1], matrix);
		poly->dots_rotz_only[2] = rotate_dot(poly->dots_rotz_only[2], matrix);
		poly->dots_rotz_only[3] = rotate_dot(poly->dots_rotz_only[3], matrix);

		// poly->equation_rotz_only.d = -(poly->equation_rotz_only.v.x * poly->dots_rotz_only[0].x +\
		// 								poly->equation_rotz_only.v.y * poly->dots_rotz_only[0].y +\
		// 								poly->equation_rotz_only.v.z * poly->dots_rotz_only[0].z);
		
		poly->i_rotz_only = (t_fdot_3d){poly->dots_rotz_only[1].x - poly->dots_rotz_only[0].x,\
										poly->dots_rotz_only[1].y - poly->dots_rotz_only[0].y,\
										poly->dots_rotz_only[1].z - poly->dots_rotz_only[0].z};
		poly->j_rotz_only = (t_fdot_3d){poly->dots_rotz_only[N_DOTS_POLY - 1].x - poly->dots_rotz_only[0].x,\
										poly->dots_rotz_only[N_DOTS_POLY - 1].y - poly->dots_rotz_only[0].y,\
										poly->dots_rotz_only[N_DOTS_POLY - 1].z - poly->dots_rotz_only[0].z};
		// poly->ii = poly->i.x * poly->i.x + poly->i.y * poly->i.y + poly->i.z * poly->i.z;
		// poly->jj = poly->j.x * poly->j.x + poly->j.y * poly->j.y + poly->j.z * poly->j.z;
		poly = poly->next;
	}
	object = map->object;
	while (object)
	{
		object->pos = rotate_dot(object->pos, matrix);
		object = object->next;
	}
}

void				copy_rotate_rotz_only(t_map *map, t_poly *poly, t_matrix matrix)
{
	t_object		*object;

	while (poly)
	{
		if (poly->object && poly->object->type == BOX && poly->object->visible == 0)
		{
			poly = poly->next;
			continue ;	//Garde l'objet devant soi pour garder l'orientation et la distance
		}
		poly->equation.v = rotate_dot(poly->equation_rotz_only.v, matrix);
		poly->equation.d = poly->equation_rotz_only.d;

		poly->dots[0] = rotate_dot(poly->dots_rotz_only[0], matrix);
		poly->dots[1] = rotate_dot(poly->dots_rotz_only[1], matrix);
		poly->dots[2] = rotate_dot(poly->dots_rotz_only[2], matrix);
		poly->dots[3] = rotate_dot(poly->dots_rotz_only[3], matrix);

		// poly->ii = poly->i.x * poly->i.x + poly->i.y * poly->i.y + poly->i.z * poly->i.z;
		// poly->jj = poly->j.x * poly->j.x + poly->j.y * poly->j.y + poly->j.z * poly->j.z;
		poly->i = (t_fdot_3d){	poly->dots[1].x - poly->dots[0].x,\
								poly->dots[1].y - poly->dots[0].y,\
								poly->dots[1].z - poly->dots[0].z};
		poly->j = (t_fdot_3d){	poly->dots[N_DOTS_POLY - 1].x - poly->dots[0].x,\
								poly->dots[N_DOTS_POLY - 1].y - poly->dots[0].y,\
								poly->dots[N_DOTS_POLY - 1].z - poly->dots[0].z};
		poly = poly->next;
	}
	object = map->object;
	while (object)
	{
		object->pos = rotate_dot(object->pos, matrix);
		object = object->next;
	}
}

// void				copy_rotate_rotz_only(t_player *player, t_poly *poly, t_matrix matrix)
// {
// 	float	i_angle = 0;

// 	matrix = player->rot_y > 0 ? player->ry : player->ry_inv;
// 	while (poly)
// 	{
// 		i_angle = 0;
// 		while (i_angle < ft_abs(player->rot_y))
// 		{
// 			poly->equation.v = rotate_dot(poly->equation_rotz_only.v, matrix);
// 			poly->equation.d = poly->equation_rotz_only.d;

// 			poly->dots[0] = rotate_dot(poly->dots_rotz_only[0], matrix);
// 			poly->dots[1] = rotate_dot(poly->dots_rotz_only[1], matrix);
// 			poly->dots[2] = rotate_dot(poly->dots_rotz_only[2], matrix);
// 			poly->dots[3] = rotate_dot(poly->dots_rotz_only[3], matrix);

// 			// poly->ii = poly->i.x * poly->i.x + poly->i.y * poly->i.y + poly->i.z * poly->i.z;
// 			// poly->jj = poly->j.x * poly->j.x + poly->j.y * poly->j.y + poly->j.z * poly->j.z;
// 			i_angle += player->ddir;
// 		}
// 			poly->i = (t_fdot_3d){	poly->dots[1].x - poly->dots[0].x,\
// 									poly->dots[1].y - poly->dots[0].y,\
// 									poly->dots[1].z - poly->dots[0].z};
// 			poly->j = (t_fdot_3d){	poly->dots[N_DOTS_POLY - 1].x - poly->dots[0].x,\
// 									poly->dots[N_DOTS_POLY - 1].y - poly->dots[0].y,\
// 									poly->dots[N_DOTS_POLY - 1].z - poly->dots[0].z};
// 		poly = poly->next;
// 	}
// }



void				translate_all(t_map *map, t_poly *poly, t_fdot_3d translation)
{
	t_object		*object;

	while (poly)
	{
		if (poly->object && poly->object->type == BOX && poly->object->visible == 0)
		{
			poly = poly->next;
			continue ;	//Garde l'objet devant soi pour garder l'orientation et la distance
		}
		translate_dot(&(poly->dots[0]), translation);
		translate_dot(&(poly->dots[1]), translation);
		translate_dot(&(poly->dots[2]), translation);
		translate_dot(&(poly->dots[3]), translation);
		poly->equation.d = -(poly->equation.v.x * poly->dots[0].x +\
								poly->equation.v.y * poly->dots[0].y +\
								poly->equation.v.z * poly->dots[0].z);
		poly = poly->next;
	}
	object = map->object;
	while (object)
	{
		translate_dot(&(object->pos), translation);
		object = object->next;
	}
}

void				translate_all_rotz_only(t_map *map, t_poly *poly, t_fdot_3d translation)
{
	t_object		*object;

	while (poly)
	{
		if (poly->object && poly->object->type == BOX && poly->object->visible == 0)
		{
			poly = poly->next;
			continue ;	//Garde l'objet devant soi pour garder l'orientation et la distance
		}
		translate_dot(&(poly->dots_rotz_only[0]), translation);
		translate_dot(&(poly->dots_rotz_only[1]), translation);
		translate_dot(&(poly->dots_rotz_only[2]), translation);
		translate_dot(&(poly->dots_rotz_only[3]), translation);
		poly->equation_rotz_only.d = -(poly->equation_rotz_only.v.x * poly->dots_rotz_only[0].x +\
										poly->equation_rotz_only.v.y * poly->dots_rotz_only[0].y +\
										poly->equation_rotz_only.v.z * poly->dots_rotz_only[0].z);
		poly = poly->next;
	}
	object = map->object;
	while (object)
	{
		translate_dot(&(object->pos), translation);
		object = object->next;
	}
}