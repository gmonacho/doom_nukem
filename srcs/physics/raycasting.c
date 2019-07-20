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
	Uint32			*pixel;
	int				x_texture;
	double			y_texture;
	double			dy_texture;

	if (!wall)
		return ;
	SDL_SetRenderDrawColor(win->rend, 0x40, 0xDD, 0x40, 255);

	win->middle_print = player->orientation + player->z - (player->shift ? player->height / 2 : 0);
	haut = HEIGHT_WALL * (wall->sector->height - player->height) / player->lenRay;
	bas = HEIGHT_WALL * player->height / player->lenRay;

	// draw_column(win, calculs->column, win->middle_print - haut,\
	//									win->middle_print + bas);

	if (wall->p1.x == wall->p2.x)
		x_texture = (int)prop(calculs->closest.y,\
					(t_dot){wall->p1.y, wall->p2.y},\
					(t_dot){0, wall->texture->pitch});
	else
		x_texture = (int)prop(calculs->closest.x,\
					(t_dot){wall->p1.x, wall->p2.x},\
					(t_dot){0, wall->texture->pitch});
	dy_texture = wall->texture->h / (bas - haut);
	y_texture = wall->texture->h;
	while (bas++ < haut)
	{
		pixel = wall->texture->pixels + (int)(y_texture * wall->texture->pitch) + x_texture;
		SDL_SetRenderDrawColor(win->rend,	(*pixel & 0xFF000000) >> 24,\
											(*pixel & 0x00FF0000) >> 16,\
											(*pixel & 0x0000FF00) >> 8,\
											*pixel & 0x000000FF);
		SDL_RenderDrawPoint(win->rend, calculs->column, bas);
		y_texture -= dy_texture;
	}
}

static t_linedef	*intersection_ray_wall(t_sector **sector, t_fdot *source, double ray_angle, t_calculs *calculs)
{
	//Dans le secteur 'sector' un rayon stocker dans 'calculs' est lance depuis 'source'
	//La fonction renvois le mur que touche le rayon
	t_linedef	*line;
	t_linedef	*wall;
	t_fdot		collision;
	double		tmpdist;

	// printf("\n---- Begin intersection ray wall ---- %p\n", *sector);
	tmpdist = -1;
	wall = NULL;
	line = (*sector)->lines;
	while (line)
	{
		//printf("Loop new wall : %p\n", line);
		//(b1 - b2) / (a2 - a1)
		collision.x = line->isequation ?\
						(calculs->ray.b - line->equation.b) /\
						(line->equation.a - calculs->ray.a) :\
						line->equation.a;
		collision.y = calculs->ray.a * collision.x + calculs->ray.b;
		//printf("Line 1 : x = %d\ty = %d\n", line->p1.x, line->p1.y);
		//printf("Line 2 : x = %d\ty = %d\n", line->p2.x, line->p2.y);
		//printf("Collision : %f\t%f\n", collision.x, collision.y);
		//printf("Source : %f\t%f\n", source->x, source->y);
		// if (calculs->column > 990)
		// {
		// 	printf("---\n");
		// 	if ((calculs->newdist = fdist(*source, collision)) < calculs->dist ||\
		// 		calculs->dist == -1)
		// 		printf("1\n");
		// 	if (sign(collision.x - source->x) == sign(cos(ray_angle)))
		// 		printf("2\n");
		// 	if ((line->p1.x <= collision.x && collision.x <= line->p2.x) ||\
		// 		(line->p2.x <= collision.x && collision.x <= line->p1.x))
		// 		printf("3\n");
		// 	if ((int)collision.x != (int)source->x || (int)collision.y != (int)source->y)
		// 		printf("4\n");
		// }

		//Va se debug seul si l'angle du rayon se comporte correctement.
		if (((calculs->newdist = fdist(*source, collision)) < tmpdist ||\
			tmpdist == -1) &&\
			sign(collision.x - source->x) == sign(cos(ray_angle)) &&\
			((line->p1.x <= collision.x && collision.x <= line->p2.x) ||\
			(line->p2.x <= collision.x && collision.x <= line->p1.x)) &&\
			((int)collision.x != (int)source->x || (int)collision.y != (int)source->y))
		{
			// if (nportals >= 1)
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
		//printf("wall = %p\tdestline = %p\tnew sector = \n", wall, wall->destline);
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
	int			nportals = 0;

	//printf("-----------------\n");
	sector = player->sector;
	source = player->pos;
	ray_angle = calculs->alpha;
	calculs->dist = 0;
	wall = intersection_ray_wall(&sector, &source, ray_angle, calculs);	
	while (wall && wall->flags & PORTAL && HEIGHT_WALL / calculs->dist > 0.05 && ++nportals)
	{
		// printf("Addr first wall : %p\t%p\n", wall, wall->destline);
		// printf("PORTAL DETECTED : %d\n", nportals);
		// printf("Angle = %fpi\ta = %f\tb = %f\n", ray_angle / M_PI, calculs->ray.a, calculs->ray.b);
		
		// printf("Wall 1 : x = %d\ty = %d\n", wall->p1.x, wall->p2.x);
		// printf("Wall 2 : x = %d\ty = %d\n", wall->destline->p2.x, wall->destline->p1.x);
		
		ray_angle -= wall->destline->angle - wall->angle +\
		(sign(wall->p2.x - wall->p1.x) == sign(wall->destline->p2.x - wall->destline->p1.x) ?\
		0 : M_PI);
		// if (wall->p2.x == wall->p1.x || wall->destline->p2.x == wall->destline->p1.x)
		// 	ray_angle -= wall->destline->angle - wall->angle +\
		// 	(sign(wall->p2.y - wall->p1.y) == sign(wall->destline->p2.y - wall->destline->p1.y) ?\
		// 	0 : M_PI);
		// else
		// 	ray_angle -= wall->destline->angle - wall->angle +\
		// 	(sign(wall->p2.x - wall->p1.x) == sign(wall->destline->p2.x - wall->destline->p1.x) ?\
		// 	0 : M_PI);

		calculs->ray.a = tan(ray_angle);
		calculs->ray.b = source.y - calculs->ray.a * source.x;
		// printf("New : Angle = %fpi\ta = %f\tb = %f\n", ray_angle / M_PI, calculs->ray.a, calculs->ray.b);
		wall = intersection_ray_wall(&sector, &source, ray_angle, calculs);
		// if (wall)
		// 	printf("Addr wall : %p\t%p\n", wall, wall->destline);
	}
	player->lenRay = calculs->dist * cos(ray_angle - player->dir);
	// player->lenRay = calculs->dist;
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
		if (cos(calculs.alpha) > 0.00001 || cos(calculs.alpha) < -0.00001)
		{
			calculs.ray.a = tan(calculs.alpha);
			calculs.ray.b = player->pos.y - calculs.ray.a * player->pos.x;
			begin_ray(win, player, &calculs);
		}
		else
			printf("No equation ray\n");
			// ;
		calculs.alpha += calculs.dangle;
	}
	//printf("Pos : %f\t%f\n", player->pos.x, player->pos.y);
	return (0);
}
