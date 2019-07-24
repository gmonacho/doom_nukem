#include "doom_nukem.h"

/*
**	ATTENTION LE REPERE POUR LES CALCULS EST AUSSI INVERSE :
**	.---->
**	|
**	v
**
**	----------------------------------------------------------------------------------------------
**	Raycasting :
**
**	Trouver la collision :	x = (b1 - b2) / (a2 - a1)
**
**	La save que si	->	Distance collision soit inferieure a celle enregistre
**					->	La collision doit etre avec un rayon qui part devant le joueur
**					->	La collision doit etre entre les 2 bords du mur
**					->	La collision n'est pas le meme point que la source
**
**	----------------------------------------------------------------------------------------------
**
**	A faire :	Textures
**				Afficher un plafond pour faire disparaitre les autres secteurs
**				Fix la tp
**				Fix les rayons
**				Attention : Reelle collision proche de la source ( < 1) entraine un mauvais choix
**				Mauvais angle apres la tp de ray !!!
**
**	-----------------------------------------------------------------------------------------------
**
**	surface = IMG_Load(file)
**
**	Milieu		: angle de vue + saut + snick
**	Haut-Milieu	: distance plafond/tete	+- distance joueur/mur
**	Milieu-Bas	: distance tete/sol		+- distance joueur/mur
*/

static void		set_ray_angle(double *ray_angle, t_linedef *wall, t_linedef *destline)
{
	if (wall->p2.x == wall->p1.x)
	{
		if (destline->p2.x == destline->p1.x)
			*ray_angle -= destline->angle - wall->angle +\
			(sign(wall->p2.y - wall->p1.y) == sign(destline->p2.y - destline->p1.y) ?\
			M_PI : 0);
		else
			*ray_angle -= destline->angle - wall->angle +\
			(sign(wall->p2.y - wall->p1.y) == sign(destline->p2.x - destline->p1.x) ?\
			0 : M_PI);
	}
	else
	{
		if (destline->p2.x == destline->p1.x)
			*ray_angle -= destline->angle - wall->angle +\
			(sign(wall->p2.x - wall->p1.x) == sign(destline->p2.y - destline->p1.y) ?\
			0 : M_PI);
		else
			*ray_angle -= destline->angle - wall->angle +\
			(sign(wall->p2.x - wall->p1.x) == sign(destline->p2.x - destline->p1.x) ?\
			M_PI : 0);
	}
}

static void		set_ray_equation(double angle, t_affine *ray, t_fdot source)
{
	if (cos(angle) > 0.0001 || cos(angle) < -0.0001)
	{
		ray->isequation = 1;
		ray->a = tan(angle);
		ray->b = source.y - ray->a * source.x;
	}
	else
		ray->isequation = 0;
}

static t_linedef	*intersection_ray_wall(t_sector **sector, t_fdot *source, double ray_angle, t_calculs *calculs)
{
	//Dans le secteur 'sector' un rayon stocker dans 'calculs' est lance depuis 'source'
	//La fonction renvois le mur que touche le rayon
	t_linedef	*line;
	t_linedef	*wall;
	t_fdot		collision;
	double		tmpdist;

	// printf("-----------------\n");
	tmpdist = -1;
	wall = NULL;
	line = (*sector)->lines;
	while (line)
	{
		// printf("Loop wall\n");
		// printf("Loop new wall : %p\n", line);
		if (line->isequation)
		{
			if (calculs->ray.isequation)
				collision.x = (calculs->ray.b - line->equation.b) /\
								(line->equation.a - calculs->ray.a);
			else
				collision.x = source->x;
		}
		else
		{
			if (calculs->ray.isequation)
				collision.x = line->equation.a;
			else
			{
				line = line->next;
				continue ;
			}
		}
		if (calculs->ray.isequation)
			collision.y = calculs->ray.a * collision.x + calculs->ray.b;
		else
			collision.y = line->equation.a * collision.x + line->equation.b;
		//printf("Line 1 : x = %d\ty = %d\n", line->p1.x, line->p1.y);
		//printf("Line 2 : x = %d\ty = %d\n", line->p2.x, line->p2.y);
		// printf("--- Line : %s\n", line->flags & PORTAL ? "PORTAL" : "WALL");
		// printf("Collision : %f\t%f\n", collision.x, collision.y);
		// printf("Source : %f\t%f\n", source->x, source->y);
		// printf("With : angle = %f pi\ta = %f\tb = %f\n", ray_angle / M_PI, calculs->ray.a, calculs->ray.b);
		// if ((calculs->newdist = fdist(*source, collision)) < tmpdist ||\
		// 	tmpdist == -1)
		// 	printf("1\n");
		// if (sign(collision.x - source->x) == sign(cos(ray_angle)))
		// 	printf("2\n");
		// if ((line->p1.x <= collision.x && collision.x <= line->p2.x) ||\
		// 	(line->p2.x <= collision.x && collision.x <= line->p1.x))
		// 	printf("3\n");
		// if ((int)collision.x != (int)source->x || (int)collision.y != (int)source->y)
		// 	printf("4\n");


		if (((calculs->newdist = fdist(*source, collision)) < tmpdist ||\
			tmpdist == -1) &&\
			sign(collision.x - source->x) == sign(cos(ray_angle)) &&\
			((line->p1.x <= collision.x && collision.x <= line->p2.x) ||\
			(line->p2.x <= collision.x && collision.x <= line->p1.x)) &&\
			((int)collision.x != (int)source->x || (int)collision.y != (int)source->y))
		{
			// if (calculs->nportals >= 1)
			// 	printf("Find collision : %f\t%f\tSource : %f\t%f\n", collision.x, collision.y, source->x, source->y);
			// printf("With : angle = %f pi\ta = %f\tb = %f\n", ray_angle / M_PI, calculs->ray.a, calculs->ray.b);
			tmpdist = calculs->newdist;
			calculs->closest = collision;
			wall = line;
		}
		line = line->next;
	}
	if (!wall)
	{
		// printf("WTTFFF ????? Column %d Wall = %p\n", calculs->column, wall);
		// printf("Source : %f\t%f\n", source->x, source->y);
		// printf("With : angle = %f pi\ta = %f\tb = %f\n", ray_angle / M_PI, calculs->ray.a, calculs->ray.b);
		line = (*sector)->lines;
		while (line)
		{
			// printf("Angle mur = %f pi\t\t a = %f\t\tb = %f\n", line->angle / M_PI, line->equation.a, line->equation.b);
			line = line->next;
		}
		// exit(1);
		return (NULL);
	}
	if (wall->flags & PORTAL)
	{
		// printf("portal detected\n");
		// printf("wall = %p\tdestline = %p\n", wall, wall->destline);
		// printf("Wall collision : p1 : %d\t%d\t p2 : %d\t%d\n", wall->p1.x, wall->p1.y, wall->p2.x, wall->p2.y);
		// printf("Wall teleporte : p1 : %d\t%d\t p2 : %d\t%d\n", wall->destline->p1.x, wall->destline->p1.y, wall->destline->p2.x, wall->destline->p2.y);
		*source = (t_fdot){prop(calculs->closest.x,\
								(t_dot){wall->p1.x, wall->p2.x},\
								(t_dot){wall->destline->p2.x, wall->destline->p1.x}),\
							prop(calculs->closest.y,\
								(t_dot){wall->p1.y, wall->p2.y},\
								(t_dot){wall->destline->p2.y, wall->destline->p1.y})};
		*sector = wall->destline->sector;
		// printf("New source : %f\t%f\n", source->x, source->y);
	}
	// printf("Tmpdist : %f\n", tmpdist);
	calculs->dist += tmpdist;
	// SDL_SetRenderDrawColor(win->rend, 0xDD, 0x40, 0x40, 255);
	// draw_line(win, (t_dot){(int)source->x, (int)source->y}, (t_dot){(int)collision.x, (int)collision.y});
	// printf("Collision : %f\t%f\n", collision.x, collision.y);
	return (wall);	
}

static void		begin_ray(t_win *win, t_player *player, t_calculs *calculs)
{
	//Lance des tests de collisions avec les murs de secteur en secteur
	//jusqu'a ce qu'il touche autre chose qu'un portal
	//Lorsqu'il touche un portail teleporte le rayon la ou il faut
	t_linedef	*wall;
	t_sector	*sector;
	t_fdot		source;
	double		ray_angle;

	//printf("-----------------\n");
	calculs->nportals = 0;
	sector = player->sector;
	source = player->pos;
	ray_angle = calculs->alpha;
	calculs->dist = 0;
	wall = intersection_ray_wall(&sector, &source, ray_angle, calculs);
	while (wall && wall->flags & PORTAL && HEIGHT_WALL / calculs->dist > 0.005 && ++calculs->nportals)
	{
		// printf("PORTAL DETECTED : %d\n", calculs->nportals);
		// printf("Addr wall : %p\t%p\n", wall, wall->destline);
		// printf("Angle = %fpi\tEquation : %d\ta = %f\tb = %f\n", ray_angle / M_PI, calculs->ray.isequation, calculs->ray.a, calculs->ray.b);
		// printf("Wall 1 : x = %d\ty = %d\n", wall->p1.x, wall->p2.x);
		// printf("Wall 2 : x = %d\ty = %d\n", wall->destline->p2.x, wall->destline->p1.x);

		// ray_angle -= wall->destline->angle - wall->angle +\
		// (sign(wall->p2.x - wall->p1.x) == sign(wall->destline->p2.x - wall->destline->p1.x) ?\
		// M_PI : 0);

		set_ray_angle(&ray_angle, wall, wall->destline);
		set_ray_equation(ray_angle, &(calculs->ray), source);
		// printf("New : Angle = %fpi\tEquation : %d\ta = %f\tb = %f\n", ray_angle / M_PI, calculs->ray.isequation, calculs->ray.a, calculs->ray.b);		
		wall = intersection_ray_wall(&sector, &source, ray_angle, calculs);
		// if (wall)
		// 	printf("Portal : %d\tAddr wall : %p\t%p\n", wall->flags & PORTAL ? 1 : 0, wall, wall->destline);
	}
	// if (calculs->closest.x < 0 || calculs->closest.y < 0)
	// {
	// 	printf("Collision : %f\t%f\n", calculs->closest.x, calculs->closest.y);
	// 	printf("New : Angle = %fpi\ta = %f\tb = %f\n", ray_angle / M_PI, calculs->ray.a, calculs->ray.b);
	// }
	// printf("Ray angle : %f\tDir : %f\n", ray_angle, player->dir);
	player->lenRay = calculs->dist/* * cos(ray_angle - player->dir)*/;
	// printf("Lenray : %f\n", player->lenRay);
	// player->lenRay = calculs->dist;
	print_wall(win, wall, player, calculs);
	// printf("Fin ray\n");
}

int				raycasting(t_win *win, t_player *player)
{
	t_calculs	calculs;

	calculs.dangle = player->fov / win->w;
	calculs.alpha = player->dir - player->fov / 2;
	calculs.column = -1;
	while (++(calculs.column) < win->w)
	{
		set_ray_equation(calculs.alpha, &(calculs.ray), player->pos);
		begin_ray(win, player, &calculs);
		calculs.alpha += calculs.dangle;
	}
	//printf("Pos : %f\t%f\n", player->pos.x, player->pos.y);
	return (0);
}