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

	//printf("Adress : %p\t%p\t%f\t%d\n", win, wall, dist, column);
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

void			find_wall(t_win *win, t_player *player, t_calculs *calculs, int column)
{
	t_linedef	*line;
	t_linedef	*wall;
	t_fdot		collision;
	t_fdot		closest;

	calculs->dist = -1;
	wall = NULL;
	line = player->sector->lines;
	while (line)
	{
		if (line->isequation)
		{
			collision.x = (calculs->ray.b - line->equation.b) /\
							(line->equation.a - calculs->ray.a);
			collision.y = calculs->ray.a * collision.x + calculs->ray.b;
			//printf("Coord : %f\t%f\n%f\t%f\n", player->pos.x, player->pos.y, collision.x, collision.y);
			if (sign(collision.x - player->pos.x) == sign(cos(calculs->alpha)) &&\
				((calculs->newdist = fdist(player->pos, collision)) < calculs->dist ||\
				calculs->dist == -1))
			{
				//printf("New closest wall : %f\t%f\n", calculs->dist, calculs->newdist);
				calculs->dist = calculs->newdist;
				closest = collision;
				wall = line;
			}
			//printf("Sings : %d\t%d\n", sign(collision.x - player->pos.x), sign(cos(calculs->alpha)));
		}
		else
			printf("No equation wall\n");
		//printf("Test wall\n");
		line = line->next;
	}
	if (wall)
		print_wall(win, wall, calculs->dist * cos(calculs->alpha - player->dir), column);
}

int				raycasting(t_win *win, t_player *player)
{
	t_calculs	calculs;
	int			column;
	
	calculs.dangle = player->fov / win->w;
	calculs.alpha = player->dir + player->fov / 2;
	//printf("Dir : %f\tLeft : %f\tRight : %f\n", player->dir, calculs.alpha, calculs.alpha - player->fov);
	column = -1;
	while (++column < win->w)
	{
		//ray = (t_fvector){cos(alpha), sin(alpha)};
		//printf("Cos : %f\n", cos(calculs.alpha));
		if (cos(calculs.alpha) > 0.05 || cos(calculs.alpha) < -0.05)
		{
			calculs.ray.a = tan(calculs.alpha);
			calculs.ray.b = player->pos.y - calculs.ray.a * player->pos.x;
			//printf("Equation : %f\t%f\n", calculs.ray.a, calculs.ray.b);
			find_wall(win, player, &calculs, column);
		}
		calculs.alpha -= calculs.dangle;
	}
	//printf("New raycasting modulo\n");
	return (0);
}
