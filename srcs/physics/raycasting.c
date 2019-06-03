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
**
**	Murs sans equations !
**	Ray sans equation !!
**
*/

void			print_wall(t_win *win, t_linedef *wall, double dist, int column)
{
	t_dot		top;
	t_dot		bottom;
	double		h;

	h = 10000 / dist;
	top = (t_dot){column, win->h / 2 - h};
	bottom = (t_dot){column, win->h / 2 + h};

	SDL_SetRenderDrawColor(win->rend, 0x40, 0x40, 0x40, 255);
	draw_line(win, (t_dot){column, 0}, top);
	draw_line(win, bottom, (t_dot){column, win->h});

	if (wall->flags & PORTAL)
		SDL_SetRenderDrawColor(win->rend, 0xDD, 0x40, 0x40, 255);
	else if (wall->flags & WALL)
		SDL_SetRenderDrawColor(win->rend, 0x40, 0xDD, 0x40, 255);
	draw_line(win, top, bottom);
}

void			find_wall(t_win *win, t_player *player, t_affine ray, int column)
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
		if (line->isequation)
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
		else
			;
		printf("Test wall\n");
	}
	print_wall(win, wall, dist, column);
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
		if (alpha != M_PI_2 && alpha != -M_PI_2)
		{
			ray.a = tan(alpha);
			ray.b = player->pos.y - ray.a * player->pos.x;
			find_wall(win, player, ray, column);
			alpha += dangle;
		}
		printf("New x\n");
	}
	return (0);
}
