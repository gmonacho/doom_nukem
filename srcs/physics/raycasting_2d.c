#include "doom_nukem.h"

/*
**	ATTENTION LE REPERE POUR LES CALCULS EST AUSSI INVERSE :
**	.---->
**	|
**	v
**
**	Line angle : (-pi/2 ; +pi/2)
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
	// int	t;

	// if (fabs(line1->angle - *ray_angle) - M_PI_2 < 0.1 * M_PI ||\
	// 	fabs(line1->angle - *ray_angle + M_PI) - M_PI_2 < 0.1 * M_PI)
	// 	t = 1;
	// else
	// 	t = 0;
	// printf("Angle line1 : %f pi\tAngle : %f pi\n", line1->angle / M_PI, *ray_angle / M_PI);
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
			*ray_angle += line2->angle - line1->angle +\
			(sign(line1->p2.x - line1->p1.x) == sign(line2->p2.x - line2->p1.x) ?\
			0 : M_PI);
			// *ray_angle -= line2->angle - line1->angle +\
			// (sign(line1->p2.x - line1->p1.x) == sign(line2->p2.x - line2->p1.x) ?\
			// M_PI : 0);
	}
	normalize(ray_angle);
	// printf("Angle line2 : %f pi\tAngle : %f pi\n\n", line2->angle / M_PI, *ray_angle / M_PI);
}

void		set_ray_equation(t_win *win, t_player *player, t_affine *ray, t_fdot source)
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
	// draw_ray(win, player, *ray);
	// printf("Angle = %fpi\tEquation : %d\ta = %f\tb = %f\n", ray->angle / M_PI, ray->isequation, ray->a, ray->b);
	win = NULL;
	player = NULL;
}

t_linedef	*intersection_ray_wall(t_win *win, t_player *player, t_fdot *source, t_sector *sector, t_calculs *calculs)
{
	//Dans le secteur 'sector' un rayon stocker dans 'calculs' est lance depuis 'source'
	//La fonction renvois le mur que touche le rayon
	t_linedef	*line;
	t_fdot		collision;
	double		tmpdist;
	double		newdirplayer;

	// printf("----------------------------\n");
	// printf("Source : %f\t%f\n", source->x, source->y);
	// printf("nportal = %d\n", calculs->nportals);
	newdirplayer = player->dir;
	// if (calculs->nportals && calculs->raycast)	//ne sert pas je crois att
	// {
	// 	set_ray_angle(&newdirplayer, calculs->collision_wall, calculs->collision_wall->destline);
	// 	// printf("Column : %d\nPlayer = %f\nNewlpayer = %f\n\n", calculs->column,\
	// 	// 								fabs(cos(player->dir - calculs->ray.angle)),\
	// 	// 								fabs(cos(newdirplayer - calculs->ray.angle)));
	// }
	calculs->collision_wall = NULL;
	tmpdist = -1;
	line = sector->lines;
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
			((line->equation.isequation &&\
			((line->p1.x <= collision.x && collision.x <= line->p2.x) ||\
			(line->p2.x <= collision.x && collision.x <= line->p1.x))) ||\
			(!line->equation.isequation &&\
			((line->p2.y <= collision.y && collision.y <= line->p1.y) ||\
			(line->p1.y <= collision.y && collision.y <= line->p2.y)))) &&\
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
			calculs->collision_wall = line;
		}
		line = line->next;
	}
	if (!calculs->collision_wall)
	{
		// printf("WTTFFF ????? Column %d calculs->collision_wall = %p\n", calculs->column, calculs->collision_wall);
		// printf("Source : %f\t%f\t%p\n", source->x, source->y, sector);
		// printf("With : angle = %f pi\ta = %f\tb = %f\n", calculs->ray.angle / M_PI, calculs->ray.a, calculs->ray.b);
		line = sector->lines;
		while (line)
		{
			// printf("Angle mur = %f pi\t\t a = %f\t\tb = %f\n", line->angle / M_PI, line->equation.a, line->equation.b);
			line = line->next;
		}
		// exit(1);
		return (NULL);
	}
	// SDL_SetRenderDrawColor(win->rend, 0xDD, 0xFF, 0xDD, 0xFF);
	// SDL_RenderDrawPoint(win->rend, collision.x, collision.y);
	if (calculs->collision_wall->flags & PORTAL)
	{
		// printf("portal detected\n");
		// printf("collision_wall = %p\tdestline = %p\n", calculs->collision_wall, wall->destline);
		// printf("collision_wall collision : p1 : %d\t%d\t p2 : %d\t%d\n", calculs->collision_wall->p1.x, calculs->collision_wall->p1.y, calculs->collision_wall->p2.x, calculs->collision_wall->p2.y);
		// printf("collision_wall teleporte : p1 : %d\t%d\t p2 : %d\t%d\n", calculs->collision_wall->destline->p1.x, calculs->collision_wall->destline->p1.y, calculs->collision_wall->destline->p2.x, calculs->collision_wall->destline->p2.y);	
		// if (collision.x < 600)
		// printf("Collision : %f\t%f\n", collision.x, collision.y);
		// printf("New source : %f\t%f\n\n", source->x, source->y);
	}
	// calculs->dist += tmpdist * (calculs->raycast ?\
	// 							fabs(cos(newdirplayer - calculs->ray.angle)) : 1);
	calculs->dist += tmpdist * (calculs->nportals == 0 && calculs->raycast ?\
								fabs(cos(newdirplayer - calculs->ray.angle)) : 1);
	// calculs->dist += tmpdist;

	// printf("Tmpdist : %f\n", tmpdist);
	// SDL_SetRenderDrawColor(win->rend, 0xDD, 0x40, 0x40, 255);
	// draw_line(win, (t_dot){(int)source->x, (int)source->y},\
	// 				(t_dot){(int)calculs->closest.x, (int)calculs->closest.y});
	// printf("Collision : %f\t%f\n", collision.x, collision.y);
	win = NULL;
	player = NULL;
	return (calculs->collision_wall);	//C'est ssaaaaalle
}

void		launch_ray_2d(t_win *win, t_player *player, t_calculs *calculs)
{
	//Lance des tests de collisions avec les murs de secteur en secteur
	//jusqu'a ce qu'il touche autre chose qu'un portal
	//Lorsqu'il touche un portail teleporte le rayon la ou il faut
	t_linedef	*wall;
	t_sector	*sector;	//useless bien reflechir car wall ou player peuvent le porter
	t_fdot		source;

	// printf("-----------------\n");
	calculs->nportals = 0;
	sector = player->sector;
	source = (t_fdot){player->pos.x, player->pos.y};
	calculs->dist = 0;
	// printf("Dir : %f\n", player->dir);
	// printf("Angle = %fpi\tEquation : %d\ta = %f\tb = %f\n", calculs->ray.angle / M_PI, calculs->ray.isequation, calculs->ray.a, calculs->ray.b);		
	wall = intersection_ray_wall(win, player, &source, sector, calculs);
	while (wall && wall->flags & PORTAL && calculs->dist < RENDER_DISTANCE)
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

		set_new_position(&(calculs->closest), wall, wall->destline, &sector);
		source = (t_fdot){calculs->closest.x, calculs->closest.y};
		set_ray_angle(&(calculs->ray.angle), wall, wall->destline);
		set_ray_equation(win, player, &(calculs->ray), source);
		// printf("New : Angle = %fpi\tEquation : %d\ta = %f\tb = %f\n", calculs->ray.angle / M_PI, calculs->ray.isequation, calculs->ray.a, calculs->ray.b);		
		wall = intersection_ray_wall(win, player, &source, sector, calculs);
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
	print_column(win, wall, player, calculs);
	// printf("Fin ray\n");
}