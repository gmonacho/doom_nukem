#include "doom_nukem.h"

/*
**	ATTENTION LE REPERE POUR LES CALCULS EST AUSSI INVERSE :
**	.---->
**	|
**	v
**
**	Line angle : (-pi/2 ; +pi/2)
**	Ray  angle  : ( ; )
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
**	A faire :	Afficher un plafond pour faire disparaitre les autres secteurs
**
**	-----------------------------------------------------------------------------------------------
**
**	Milieu		: angle de vue + saut + snick
**	Haut-Milieu	: distance plafond/tete	+- distance joueur/mur
**	Milieu-Bas	: distance tete/sol		+- distance joueur/mur
*/

void		set_ray_angle(double *ray_angle, t_linedef *line1, t_linedef *line2)
{
	// printf("Angle : %f\n", *ray_angle);
	if (line1->p2.x == line1->p1.x)
	{
		if (line2->p2.x == line2->p1.x)
			*ray_angle -= line2->angle - line1->angle +\
			(sign(line1->p2.y - line1->p1.y) == sign(line2->p2.y - line2->p1.y) ?\
			M_PI : 0);
		else
			*ray_angle -= line2->angle - line1->angle +\
			(sign(line1->p2.y - line1->p1.y) == sign(line2->p2.x - line2->p1.x) ?\
			0 : M_PI);
	}
	else
	{
		if (line2->p2.x == line2->p1.x)
			*ray_angle -= line2->angle - line1->angle +\
			(sign(line1->p2.x - line1->p1.x) == sign(line2->p2.y - line2->p1.y) ?\
			0 : M_PI);
		else
			*ray_angle -= line2->angle - line1->angle +\
			(sign(line1->p2.x - line1->p1.x) == sign(line2->p2.x - line2->p1.x) ?\
			M_PI : 0);
	}
	// printf("Angle : %f\n\n", *ray_angle);
}

static void		set_ray_equation(t_win *win, t_player *player, t_affine *ray, t_fdot source)
{
	// printf("Angle = %fpi\tEquation : %d\ta = %f\tb = %f\n", ray->angle / M_PI, ray->isequation, ray->a, ray->b);
	if (cos(ray->angle) > 0.00001 || cos(ray->angle) < -0.00001)
	{
		ray->isequation = 1;
		ray->a = tan(ray->angle);
		ray->b = source.y - ray->a * source.x;
		// printf("Angle : %f\tCos : %f\tEquation : a = %f\tb = %f\n", ray->angle, cos(ray->angle), ray->a, ray->b);
	}
	else
	{
		ray->isequation = 0;
		ray->a = source.x;
	}
	win = NULL;
	player = NULL;
	// draw_ray(win, player, *ray);
	// printf("Angle = %fpi\tEquation : %d\ta = %f\tb = %f\n", ray->angle / M_PI, ray->isequation, ray->a, ray->b);
}

static t_linedef	*intersection_ray_wall(t_win *win, t_player *player, t_sector **sector, t_fdot *source, t_calculs *calculs)
{
	//Dans le secteur 'sector' un rayon stocker dans 'calculs' est lance depuis 'source'
	//La fonction renvois le mur que touche le rayon
	t_linedef	*line;
	t_linedef	*wall;
	t_fdot		collision;
	double		tmpdist;

	// printf("----------------------------\n");
	// printf("Source : %f\t%f\n", source->x, source->y);
	tmpdist = -1;
	wall = NULL;
	line = (*sector)->lines;
	while (line)
	{
		if (!lines_intersection(&collision, &(line->equation), &(calculs->ray)))
		{
			line = line->next;	
			continue ;
		}
		// if (collision.y > 10000 || collision.y < 10000)
		// {
		// 	printf("Line 1 : x = %d\ty = %d\n", line->p1.x, line->p1.y);
		// 	printf("Line 2 : x = %d\ty = %d\n", line->p2.x, line->p2.y);
		// }
		// printf("--- Line : %s\n", line->flags & PORTAL ? "PORTAL" : "WALL");
		// printf("Collision : %f\t%f\n", collision.x, collision.y);
		// printf("With : angle = %f pi\ta = %f\tb = %f\n", calculs->ray.angle / M_PI, calculs->ray.a, calculs->ray.b);
		
		// if (calculs->nportals >= 2)
		// {
			// printf("-\n");
			// if ((calculs->newdist = fdist(*source, collision)) < tmpdist ||\
			// 	tmpdist == -1)
			// 	printf("1\n");
			// if (sign(collision.x - source->x) == sign(cos(calculs->ray.angle)))
			// 	printf("2\n");
			// if ((line->p1.x <= collision.x && collision.x <= line->p2.x) ||\
			// 	(line->p2.x <= collision.x && collision.x <= line->p1.x))
			// 	printf("3\n");
			// if ((int)collision.x != (int)source->x || (int)collision.y != (int)source->y)
			// 	printf("4 : %f\t%f : %u\n", collision.x, collision.y, line->flags);
		// }

		if (((calculs->newdist = fdist(*source, collision)) < tmpdist ||\
			tmpdist == -1) &&\
			sign(collision.x - source->x) == sign(cos(calculs->ray.angle)) &&\
			((line->p1.x <= collision.x && collision.x <= line->p2.x) ||\
			(line->p2.x <= collision.x && collision.x <= line->p1.x)) &&\
			// (collision.x != source->x || collision.y != source->y))
			((int)collision.x != (int)source->x || (int)collision.y != (int)source->y))
			// (10 * (int)collision.x != 10 * (int)source->x || 10 * (int)collision.y != 10 * (int)source->y))
			// (100 * (int)collision.x != 100 * (int)source->x || 100 * (int)collision.y != 100 * (int)source->y))
		{
			// if (calculs->nportals >= 1)
			// 	printf("Find collision : %f\t%f\tSource : %f\t%f\n", collision.x, collision.y, source->x, source->y);
			// printf("With : angle = %f pi\ta = %f\tb = %f\n", calculs->ray.angle / M_PI, calculs->ray.a, calculs->ray.b);
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
		// printf("With : angle = %f pi\ta = %f\tb = %f\n", calculs->ray.angle / M_PI, calculs->ray.a, calculs->ray.b);
		line = (*sector)->lines;
		while (line)
		{
			// printf("Angle mur = %f pi\t\t a = %f\t\tb = %f\n", line->angle / M_PI, line->equation.a, line->equation.b);
			line = line->next;
		}
		// exit(1);
		return (NULL);
	}
	win = NULL;
	// SDL_SetRenderDrawColor(win->rend, 0xDD, 0xFF, 0xDD, 0xFF);
	// SDL_RenderDrawPoint(win->rend, collision.x, collision.y);
	if (wall->flags & PORTAL || wall->flags == 2)
	{
		// printf("portal detected\n");
		// printf("wall = %p\tdestline = %p\n", wall, wall->destline);
		// printf("Wall collision : p1 : %d\t%d\t p2 : %d\t%d\n", wall->p1.x, wall->p1.y, wall->p2.x, wall->p2.y);
		// printf("Wall teleporte : p1 : %d\t%d\t p2 : %d\t%d\n", wall->destline->p1.x, wall->destline->p1.y, wall->destline->p2.x, wall->destline->p2.y);
		
		// if (collision.x < 600)
		// printf("Collision : %f\t%f\n", collision.x, collision.y);
		set_new_position(&(calculs->closest), wall, wall->destline, sector);
		*source = (t_fdot){calculs->closest.x, calculs->closest.y};
		// printf("New source : %f\t%f\n\n", source->x, source->y);
		calculs->dist += tmpdist;
	}
	else
		calculs->dist += tmpdist;// * cos(player->dir - calculs->ray.angle);
	player = NULL;
	// printf("Tmpdist : %f\n", tmpdist);
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

	// printf("-----------------\n");
	calculs->nportals = 0;
	sector = player->sector;
	source = player->pos;
	calculs->dist = 0;
	// printf("Angle = %fpi\tEquation : %d\ta = %f\tb = %f\n", calculs->ray.angle / M_PI, calculs->ray.isequation, calculs->ray.a, calculs->ray.b);		
	wall = intersection_ray_wall(win, player, &sector, &source, calculs);
	while (wall && wall->flags & PORTAL && HEIGHT_WALL / calculs->dist > 0.005)
	{
		(calculs->nportals)++;
		// if (calculs->nportals > 5)
		// 	exit(0);
		// printf("PORTAL DETECTED : %d\n", calculs->nportals);
		
		// printf("Source : %f\t%f\n", source.x, source.y);
		// printf("Addr wall : %p\t%p\n", wall, wall->destline);
		// printf("Wall 1 : x = %d\ty = %d\n", wall->p1.x, wall->p2.x);
		// printf("Wall 2 : x = %d\ty = %d\n", wall->destline->p2.x, wall->destline->p1.x);

		// calculs->ray.angle -= wall->destline->angle - wall->angle +\
		// (sign(wall->p2.x - wall->p1.x) == sign(wall->destline->p2.x - wall->destline->p1.x) ?\
		// M_PI : 0);

		set_ray_angle(&(calculs->ray.angle), wall, wall->destline);
		set_ray_equation(win, player, &(calculs->ray), source);
		// printf("New : Angle = %fpi\tEquation : %d\ta = %f\tb = %f\n", calculs->ray.angle / M_PI, calculs->ray.isequation, calculs->ray.a, calculs->ray.b);		
		wall = intersection_ray_wall(win, player, &sector, &source, calculs);
		// if (wall)
		// 	printf("Portal : %d\tAddr wall : %p\t%p\n", wall->flags & PORTAL ? 1 : 0, wall, wall->destline);
	}
	// if (calculs->closest.x < 0 || calculs->closest.y < 0)
	// {
	// 	printf("Collision : %f\t%f\n", calculs->closest.x, calculs->closest.y);
	// 	printf("New : Angle = %fpi\ta = %f\tb = %f\n", calculs->ray.angle / M_PI, calculs->ray.a, calculs->ray.b);
	// }
	// printf("Ray angle : %f\tDir : %f\n", calculs->ray.angle, player->dir);
	player->lenRay = calculs->dist/* * cos(calculs->ray.angle - player->dir)*/;
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
		// printf("---------------------\n");
		calculs.ray.angle = calculs.alpha;
		set_ray_equation(win, player, &(calculs.ray), player->pos);
		// printf("Player : %f\t%f\n", player->pos.x, player->pos.y);
		// printf("Source : %f\t%f\n\n", player->pos.y, calculs.ray.a * player->pos.x + calculs.ray.b);
		
		begin_ray(win, player, &calculs);
		calculs.alpha += calculs.dangle;
	}
	//printf("Pos : %f\t%f\n", player->pos.x, player->pos.y);
	return (0);
}