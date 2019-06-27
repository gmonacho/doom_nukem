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
**
**	----------------------------------------------------------------------------------------------
**
**	A faire :	Hauteur des murs en fonction de la hauteur du plafond/sol/personnage
**				Textures
**				Dessiner les secteurs du plus loin au plus pres
**				Afficher un plafond pour faire disparaitre les autres secteurs
**				Teleporter les rayons dans les autres secteurs
**
**	-----------------------------------------------------------------------------------------------
**
**	surface = IMG_Load(file)
**
**	Milieu		: angle de vue + saut + snick
**	Haut-Milieu	: distance plafond/tete	+- distance joueur/mur
**	Milieu-Bas	: distance tete/sol		+- distance joueur/mur
*/


static void			print_wall(t_win *win, t_linedef *wall, t_player *player, t_calculs *calculs)
{
	double			haut;
	double			bas;

	SDL_SetRenderDrawColor(win->rend, 0x40, 0xDD, 0x40, 255);

	win->middle_print = 1 * win->h / 2;
	haut = 100 * (wall->sector->height - player->height) / player->lenRay;
	bas = 100 * player->height / player->lenRay;

	draw_column(win, calculs->column, win->middle_print - haut,\
										win->middle_print + bas);
}

static t_linedef	*intersection_ray_wall(t_sector **sector, t_fdot *source, double ray_angle, t_calculs *calculs)
{
	//Dans le secteur 'sector' un rayon stocker dans 'calculs' est lance depuis 'source'
	//La fonction renvois le mur touche par le rayon
	t_linedef	*line;
	t_linedef	*wall;
	t_fdot		collision;
	t_fdot		closest;

	//printf("\n---- Begin intersection ray wall ----\n");
	calculs->dist = -1;
	wall = NULL;
	line = (*sector)->lines;
	while (line)
	{
		//printf("Loop new wall : %p\n", line);
		if (line->isequation)
		{
			//(b1 - b2) / (a2 - a1)
			collision.x = (calculs->ray.b - line->equation.b) /\
							(line->equation.a - calculs->ray.a);
			collision.y = line->equation.a * collision.x + line->equation.b;
			//printf("Line 1 : x = %d\ty = %d\n", line->p1.x, line->p1.y);
			//printf("Line 2 : x = %d\ty = %d\n", line->p2.x, line->p2.y);
			//printf("Collision : %f\t%f\n", collision.x, collision.y);
			//printf("Source : %f\t%f\n", source->x, source->y);
			if (((calculs->newdist = fdist(*source, collision)) < calculs->dist ||\
				calculs->dist == -1) &&\
				sign(collision.x - source->x) == sign(cos(ray_angle)) &&\
				((line->p1.x <= collision.x && collision.x <= line->p2.x) ||\
				(line->p2.x <= collision.x && collision.x <= line->p1.x)))
			{
				calculs->dist = calculs->newdist;
				closest = collision;
				wall = line;
			}
		}
		else
			printf("No equation wall\n");
		line = line->next;
	}
	if (!wall)
	{
		printf("WTTFFF ????? Column %d Wall = %p\n", calculs->column, wall);
		printf("Angle = %fpi\ta = %f\tb = %f\n", ray_angle / M_PI, calculs->ray.a, calculs->ray.b);
		printf("calculs->dist = %f\n", calculs->dist);
		exit(1);
	}
	if (wall->flags & PORTAL)
	{
		//printf("wall = %p\tdestline = %p\tnew sector = \n", wall, wall->destline);
		*source = collision;
		*sector = wall->destline->sector;
	}
	return (wall);	
}

static void		begin_ray(t_win *win, t_player *player, t_calculs *calculs)
{
	//Teleporte le rayon et lance des tests de collisions avec les murs de secteur en secteur
	//jusqu'a ce qu'il touche autre chose qu'un portal
	t_linedef	*wall;
	t_sector	*sector;
	t_fdot		source;
	double		ray_angle;

	sector = player->sector;
	source = player->pos;
	ray_angle = calculs->alpha;
	wall = intersection_ray_wall(&sector, &source, ray_angle, calculs);
	//printf("Player : %f\t%f\n", player->pos.x, player->pos.y);
	/*while ((wall = intersection_ray_wall(&sector, source, ray_angle, calculs))->flags & PORTAL)
	{
		printf("PORTAL DETECTED\n");
		printf("Angle = %f\ta = %f\tb = %f\n", ray_angle, calculs->ray.a, calculs->ray.b);
		ray_angle -= wall->destline->angle - wall->angle +\
		(sign(wall->p2.x - wall->p1.x) == sign(wall->destline->p2.x - wall->destline->p1.x) ?\
		M_PI : 0);
		calculs->ray.a = tan(ray_angle);
		calculs->ray.b = player->pos.y - calculs->ray.a * player->pos.x;
		printf("Angle = %f\ta = %f\tb = %f\n", ray_angle, calculs->ray.a, calculs->ray.b);
	}*/
	player->lenRay = calculs->dist * cos(ray_angle - player->dir);
	print_wall(win, wall, player, calculs);
}

int				raycasting(t_win *win, t_player *player)
{
	t_calculs	calculs;
			
	calculs.dangle = player->fov / win->w;
	calculs.alpha = player->dir - player->fov / 2;
	calculs.column = -1;
	while (++(calculs.column) < win->w)
	{
		/*if (cos(calculs.alpha) > 0.00001 || cos(calculs.alpha) < -0.00001)
		{*/
			calculs.ray.a = tan(calculs.alpha);
			calculs.ray.b = player->pos.y - calculs.ray.a * player->pos.x;
			begin_ray(win, player, &calculs);
		/*}
		else
			;//printf("No equation ray\n");*/
		calculs.alpha += calculs.dangle;
	}
	//printf("Pos : %f\t%f\n", player->pos.x, player->pos.y);
	return (0);
}
