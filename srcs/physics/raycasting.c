#include "doom_nukem.h"

/*
**	ATTENTION LE REPERE POUR LES CALCULS EST AUSSI INVERSE :
**	.---->
**	|
**	v
*/

void			print_wall(t_win *win, t_linedef *wall, double dist, int column)
{
	double		h;

	//printf("Adress : %p\t%p\t%f\t%d\n", win, wall, dist, column);
	h = 20000 / dist;
	if (wall->flags & PORTAL)
		SDL_SetRenderDrawColor(win->rend, 0xDD, 0x40, 0x40, 255);
	else if (wall->flags & WALL)
		SDL_SetRenderDrawColor(win->rend, 0x40, 0xDD, 0x40, 255);
	else
		SDL_SetRenderDrawColor(win->rend, 0x40, 0x40, 0xDD, 255);
	draw_column(win, column, win->h / 2 - h, win->h / 2 + h);
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
			collision.y = line->equation.a * collision.x + line->equation.b;
			if (((calculs->newdist = fdist(player->pos, collision)) < calculs->dist ||\
				calculs->dist == -1) &&\
				sign(collision.x - player->pos.x) == sign(cos(calculs->alpha)))
			{
				calculs->dist = calculs->newdist;
				closest = collision;
				wall = line;
			}
		}
		else
			printf("No equation wall\n");
		//printf("Test wall\n");
		line = line->next;
	}
	if (wall)
		print_wall(win, wall, calculs->dist * cos(calculs->alpha - player->dir), column);
	SDL_SetRenderDrawColor(win->rend, 0, 0, 0, 255);
	draw_line(win, (t_dot){(int)player->pos.x, (int)player->pos.y}, (t_dot){(int)closest.x, (int)closest.y});
}

int				raycasting(t_win *win, t_player *player)
{
	t_calculs	calculs;
	int			column;
			
	calculs.dangle = player->fov / win->w;
	calculs.alpha = player->dir - player->fov / 2;
	column = -1;
	while (++column < win->w)
	{
		/*if (cos(calculs.alpha) > 0.00001 || cos(calculs.alpha) < -0.00001)
		{*/
			calculs.ray.a = tan(calculs.alpha);
			calculs.ray.b = player->pos.y - calculs.ray.a * player->pos.x;
			find_wall(win, player, &calculs, column);
		/*}
		else
			;//printf("No equation ray\n");*/
		calculs.alpha += calculs.dangle;
	}
	//printf("Pos : %f\t%f\n", player->pos.x, player->pos.y);
	return (0);
}
