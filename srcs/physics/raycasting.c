#include "doom_nukem.h"

/*
**	-	BSP :
**
**	-	Position relative de 2 murs :
**	1 equation de droite
**	2 points pour le segment
**	On trouve la projection verticale des deux points sur la droite
**	Si les y des projections sont respectivement aux dessus des points alors le segment est en dessous
**	Si les y des projections sont respectivement en dessous des points alors le segment est au dessus
**	Si un est en dessous et l'autre au dessus alors la droite coupe le segment
*/

void			print_wall(t_linedef *wall, t_fdot collision, double dist)
{
	wall = NULL;
	collision.x = 0;
	dist = 0;
}

void			find_wall(t_player *player, t_affine ray)
{
	t_linedef	*line;
	t_linedef	*wall;
	t_fdot		collision;
	t_fdot		closest;
	double		dist;
	double		newdist;

	line = player->sector->lines;
	while (line)
	{
		collision.x = (ray.b - line->equation.b) /\
						(line->equation.a - ray.a);
		collision.y = ray.a * collision.x + ray.b;
		if (sign(collision.x - player->pos.x) == sign(cos(player->dir)) &&\
			(newdist = fdist(player->pos, collision)) < dist)
		{
			dist = newdist;
			closest = collision;
			wall = line;
		}
		line = line->next;
	}
	print_wall(wall, closest, dist);
}

int				raycasting(t_win *win, t_player *player)
{
	double		dangle;
	double		alpha;
	int			column;
	t_affine	ray;

	dangle = player->fov / win->w;
	alpha = player->dir - player->fov / 2;
	column = -1;
	while (++column < win->w)
	{
		//ray = (t_fvector){cos(alpha), sin(alpha)};
		ray.a = tan(alpha);
		ray.b = player->pos.y - ray.a * player->pos.x;
		find_wall(player, ray);
		alpha += dangle;
	}
	return (0);
}
