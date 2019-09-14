#include "doom_nukem.h"

/*
**	Antoine t'es un encule le cercle trigo EST INVERSE oublie pas, wtf t debile
**
**	Ajouts :
**	- Collisions sur les cotes des la hitbox
**	- Gliss sur les murs
**
**	s = perimetre / 2 = somme des trois cotes / 2
**	On melange 2 formules sur l'aire du triangle : base * h / 2 = sqrt(s(s-a)(s-b)(s-c))
**	Et boom on a h par rapport aux 3 cotes.
**
**
**	Nouveau systeme collisions :
**	-	Raycasting dans la direction physique du player
**	-	de la largeur de la hitbox si possible
**	-	Il faut inverser le cercle trigo pour que l'angle soit coherent
**			avec le repere.
*/

// static int	collisions(t_win *win, t_map *map, t_player *player, t_linedef *line)
static int	collisions(t_win *win, t_map *map, t_player *player)
{
	t_calculs calculs;
	t_linedef	*collision;

	calculs.raycast = 0;
	// printf("Vel = %f\t%f\n", player->vel.x, player->vel.y);
	if (player->vel.x)
		calculs.ray.angle = atan(player->vel.y / player->vel.x);
	else
		calculs.ray.angle = M_PI_2;

	// printf("Angle 1 : %f pi\n", calculs.ray.angle / M_PI);
	if (player->vel.x < 0)
		calculs.ray.angle += M_PI;
	else if (calculs.ray.angle < 0)
		calculs.ray.angle += _2_PI;

	// if (player->vel.x)
	// 	calculs.ray.angle = atan(fabs(player->vel.y / player->vel.x));
	// else
	// 	calculs.ray.angle = M_PI_2;
	// if (player->vel.x)
	// 	calculs.ray.angle = atan(fabs(player->vel.y / player->vel.x));
	// else
	// 	calculs.ray.angle = M_PI_2;
	// // printf("Angle 1 : %f pi\n", calculs.ray.angle / M_PI);
	// if (player->vel.x > 0)
	// {
	// 	if (player->vel.y > 0)
	// 		calculs.ray.angle = calculs.ray.angle;
	// 	else
	// 		calculs.ray.angle = _2_PI - calculs.ray.angle;
	// }
	// else
	// {
	// 	if (player->vel.y > 0)
	// 		calculs.ray.angle = M_PI - calculs.ray.angle;
	// 	else
	// 		calculs.ray.angle = M_PI + calculs.ray.angle;
	// }


	set_ray_equation(win, player, &(calculs.ray), player->pos);
	calculs.dist = 0;
	collision = intersection_ray_wall(win, player, &player->pos, player->sector, &calculs);

	// printf("Dist wall : %f\t%p\n", calculs.dist, collision);
	// printf("Angle 2 : %f pi\ta = %f\tb = %f\n", calculs.ray.angle / M_PI, calculs.ray.a, calculs.ray.b);
	if (calculs.dist < player->width_2 && collision)
		return (actions(win, map, collision, calculs.dist));
	// printf("\n\n");
	return (0);
}

// static int	collisions(t_win *win, t_map *map, t_linedef *line)
// {
// 	t_dot	newpos;
// 	double	a;
// 	double	b;
// 	double	c;
// 	double	h;

// 	h = 500;
// 	// printf("---------\n");
// 	newpos = (t_dot){(int)map->player.pos.x + map->player.vel.x,\
// 						(int)map->player.pos.y + map->player.vel.y};
	
// 	a = dist(line->p1, line->p2);
// 	b = dist(line->p2, newpos);
// 	c = dist(newpos, line->p1);
// 	// printf("a = %f\tb = %f\tc = %f\n", a, b, c);
	
// 	/*
// 	h = (a + b + c) / 2;
// 	h = (2 * sqrt(h * (h - a) * (h - b) * (h - c))) / a;	//Old version
// 	if (h < map->player.width / 2 + map->player.const_vel)
// 		return (actions(win, map, line, h));
// 	*/
	
// 	h = b + c - a;
// 	if (h < map->player.width_10)
// 		return (actions(win, map, line, h));
// 	// printf("Dist wall player : %f\n", h);
// 	return (0);
// }

// static int	collisions(t_win *win, t_map *map, t_linedef *line, t_affine *traj)
// {
// 	t_player	*player;
// 	t_fdot		collision;

// 	player = &(map->player);
	
// 	if (lines_intersection(&collision, &(line->equation), traj))
// 		return (0);



// 	if ((line->isequation &&\
// 		((line->p1.x <= collision.x && collision.x <= line->p2.x) ||\
// 		(line->p2.x <= collision.x && collision.x <= line->p1.x))) ||\
// 		(!(line->isequation) &&\
// 		((line->p1.y <= collision.y && collision.y <= line->p2.y) ||\
// 		(line->p2.y <= collision.y && collision.y <= line->p1.y))))
// 	{
// 		if (player->vel.x > 0)
// 		{
// 			if (player->vel.y > 0)
// 			{
// 				if (player->pos.x < collision.x &&\
// 					collision.x <= player->pos.x + player->vel.x + player->width / 2 && 
// 					player->pos.y < collision.y &&\
// 					collision.y <= player->pos.y + player->vel.y + player->width / 2)
// 					return (actions(win, map, line, player));
// 			}
// 			else
// 			{
// 				if (player->pos.x < collision.x &&\
// 					collision.x <= player->pos.x + player->vel.x + player->width / 2 &&\
// 					player->pos.y > collision.y &&\
// 					collision.y >= player->pos.y + player->vel.y - player->width / 2)
// 					return (actions(win, map, line, player));
// 			}
// 		}
// 		else
// 		{
// 			if (player->vel.y > 0)
// 			{
// 				if (player->pos.x > collision.x &&\
// 					collision.x >= player->pos.x + player->vel.x - player->width / 2 &&\
// 					player->pos.y < collision.y &&\
// 					collision.y <= player->pos.y + player->vel.y + player->width / 2)
// 					return (actions(win, map, line, player));
// 			}
// 			else
// 			{
// 				if (player->pos.x > collision.x &&\
// 					collision.x >= player->pos.x + player->vel.x - player->width / 2 &&\
// 					player->pos.y > collision.y &&\
// 					collision.y >= player->pos.y + player->vel.y - player->width / 2)
// 					return (actions(win, map, line, player));
// 			}
// 		}
// 	}
// 	return (0);
// }

// static int	collisions(t_map *map, t_linedef *line, t_affine traj,\
// 						t_player *player)
// {
// 	t_fdot	collision;

// 	if (traj.a != line->equation.a)
// 	{
// 		if (line->isequation)
// 			collision.x = (line->equation.b - traj.b) /\
// 							(traj.a - line->equation.a);
// 		else
// 			collision.x = line->equation.a;
// 		collision.y = traj.a * collision.x + traj.b;
// 		if ((line->isequation &&\
// 			((line->p1.x <= collision.x && collision.x <= line->p2.x) ||\
// 			(line->p2.x <= collision.x && collision.x <= line->p1.x))) ||\
// 			(!(line->isequation) &&\
// 			((line->p1.y <= collision.y && collision.y <= line->p2.y) ||\
// 			(line->p2.y <= collision.y && collision.y <= line->p1.y))))
// 		{
// 			if (fdist(player->pos, collision) <\
// 									fmag(player->vel) + player->width / 2 &&\
// 				fdist(collision, (t_fdot){player->pos.x + player->vel.x,\
// 										player->pos.y + player->vel.y}) <\
// 									fmag(player->vel))
// 				return (actions(map, line, player));
// 		}
// 	}
// 	return (0);
// }

int		physics(t_win *win, t_map *map, t_player *player)
{
	//int			i;
	//t_sector	*sector;
	// t_linedef	*line;
	// t_affine	traj;

	//printf("\nPos : %f\t%f\n", player->pos.x, player->pos.y);
	//printf("Vel : %f\t%f\n", player->vel.x, player->vel.y);
	if (player->z > 40)
		player->jump = 0;
	if (player->z > 5)
		player->z -= 5;
	else
		player->jump = 1;
	if (!player->vel.x && !player->vel.y)
		return (0);
	
	/*
	** --------------------------------------------------------------------------
	*/
	
	// if ((int)(player->vel.x * 100))
	// {
	// 	traj.isequation = 1;
	// 	traj.a = player->vel.y / player->vel.x;
	// 	traj.b = player->pos.y - traj.a * player->pos.x;
	// }
	// else
	// 	traj.isequation = 0;
	
	// line = player->sector->lines;
	// while (line)
	// {
	// 	if (!(line->flags & DOOR_OPEN) &&\
	// 		collisions(win, map, player, line))
	// 		break ;
	// 	line = line->next;
	// }
	collisions(win, map, player);
	
	player->pos = (t_fdot){player->pos.x + player->vel.x,\
							player->pos.y + player->vel.y};
	return (0);
}









































SDL_bool	is_in_rect(SDL_Rect rect, t_dot p)
{
	int		tmp;
	
	if (rect.w < 0)
	{
		tmp = rect.x;
		rect.x = rect.x + rect.w;
		rect.w = tmp - rect.x;
	}
	if (rect.h < 0)
	{
		tmp = rect.y;
		rect.y = rect.y + rect.h;
		rect.h = tmp - rect.y;
	}
	return (p.x > rect.x &&
			p.x < rect.x + rect.w &&
			p.y > rect.y &&
			p.y < rect.y + rect.h);
}

SDL_bool	intersect_line_rect(t_line line, SDL_Rect rect)
{
	int		tmp;
	if (rect.w < 0)
	{
		tmp = rect.x;
		rect.x = rect.x + rect.w;
		rect.w = tmp - rect.x;
	}
	if (rect.h < 0)
	{
		tmp = rect.y;
		rect.y = rect.y + rect.h;
		rect.h = tmp - rect.y;
	}
	return (SDL_IntersectRectAndLine(&rect, &line.p1.x, &line.p1.y, &line.p2.x, &line.p2.y));
}

SDL_bool	is_next_point(t_dot dot, t_dot other, int distance)
{
	t_dot	vector;
	int		vnorme;

	vector.x = dot.x - other.x;
	vector.y = dot.y - other.y;
	vnorme = sqrt(vector.x * vector.x + vector.y * vector.y);
	return (vnorme <= distance);
}