#include "doom_nukem.h"

/*
**	Il ne faut pas allonger l'image en largeur mais la repete
**	En hauteur aussi
**
**	player->orientation est le milieu y du mur sur lecran,
**	On +/- le y du perso et le snick.
**	La vision du perso retrecie en haut et en bas (On diminue HEIGHT_WALL)
**	La taille du mur depend de la distance jusqua lui (1 / lenray)
**	On alonge ou retrecie haut et bas selon
**		les diff de hauteurs z entre le secteur et le perso
**
*/
static void	draw_map(t_win *win, t_map *map)
{
	t_sector	*sector;
	t_linedef	*line;
	t_dot		x;
	t_dot		y;
	t_dot		width;
	t_dot		height;

	SDL_SetRenderDrawColor(win->rend, 0, 0, 0, 255);
	width = (t_dot){0, win->w};
	height = (t_dot){0, win->h};
	x = (t_dot){10000000, -100000000};
	y = (t_dot){10000000, -100000000};
	sector = map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			if (line->p1.x < x.x)
				x.x = line->p1.x;
			if (line->p2.x < x.x)
				x.x = line->p2.x;
			if (line->p1.x > x.y)
				x.y = line->p1.x;
			if (line->p2.x > x.y)
				x.y = line->p2.x;
			if (line->p1.y < y.x)
				y.x = line->p1.y;
			if (line->p2.y < y.x)
				y.x = line->p2.y;
			if (line->p1.y > y.y)
				y.y = line->p1.y;
			if (line->p2.y > y.y)
				y.y = line->p2.y;
			line = line->next;
		}
		sector = sector->next;
	}
	// x = (t_dot){0, win->w};	//Pour annuler le rescale
	// y = (t_dot){0, win->h};
	sector = map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			// p1 = ;
			draw_line(win, (t_dot){prop(line->p1.x, x, width),\
									prop(line->p1.y, y, height)},\
							(t_dot){prop(line->p2.x, x, width),\
									prop(line->p2.y, y, height)});
			line = line->next;
		}
		sector = sector->next;
	}

	draw_circle(win, (t_circle){prop(map->player.pos.x, x, width),\
							prop(map->player.pos.y, y, height), map->player.width / 2});
	draw_line(win,	(t_dot){prop(map->player.pos.x, x, width),\
							prop(map->player.pos.y, y, height)},\
					(t_dot){prop(map->player.pos.x + 30 * cos(map->player.dir), x, width),\
							prop(map->player.pos.y + 30 * sin(map->player.dir), y, height)});

}

/*void	draw_sprite(t_player *player, t_object *object)
{
	while (object)
	{

		object = object->next;
	}
}*/

void			print_wall(t_win *win, t_linedef *wall, t_player *player, t_calculs *calculs)
{
	double			haut;
	double			bas;
	Uint32			pixel;
	int				x_texture;
	double			y_texture;
	double			dy_texture;

	if (!wall)
	{
		// printf("WTTFFF ????? Column %d Wall = %p\n", calculs->column, wall);
		return ;
	}
	if (!wall->texture)
	{
		printf("Pas de texture sur le mur\n");
		exit(0);
	}
	if (!wall->texture->pixels)
	{
		printf("Impossible d'acceder a void* 'pixel' de la texture du mur\n");
		exit(0);
	}

	win->middle_print = player->orientation - (player->shift ? player->height / 2 : 0);
	if (player->orientation < win->h / 2)
	{
		haut = prop(player->orientation, (t_dot){0, win->h / 2}, (t_dot){3 * HEIGHT_WALL / 4, HEIGHT_WALL});
		bas = prop(player->orientation, (t_dot){0, win->h / 2}, (t_dot){3 * HEIGHT_WALL / 4, HEIGHT_WALL});
	}
	else
	{
		haut = prop(player->orientation, (t_dot){win->h / 2, win->h}, (t_dot){HEIGHT_WALL, 3 * HEIGHT_WALL / 4});
		bas = prop(player->orientation, (t_dot){win->h / 2, win->h}, (t_dot){HEIGHT_WALL, 3 * HEIGHT_WALL / 4});
	}
	haut *= -(wall->sector->ceil_height - (player->height + player->z + player->sector->floor_height)) / player->lenRay;
	bas *= (player->height + player->z + (player->sector->floor_height - wall->sector->floor_height)) / player->lenRay;
	// printf("Lenray : %f\n", player->lenRay);
	// printf("Column : %d\tHaut : %f\tBas : %f\n", calculs->column, haut, bas);
	// printf("Calcul haut : %d\n", wall->sector->ceil_height - (player->height + player->sector->floor_height));
	// printf("Bas : %d\n", player->height + (player->sector->floor_height - wall->sector->floor_height));
	// SDL_SetRenderDrawColor(win->rend, 0x40, 0xDD, 0x40, 255);
	// draw_column(win, calculs->column, win->middle_print - haut,\
	// 									win->middle_print + bas);

	if (wall->p1.x == wall->p2.x)
		x_texture = (int)prop(calculs->closest.y,\
					(t_dot){wall->p1.y, wall->p2.y},\
					(t_dot){0, wall->texture->w - 1});
	else
		x_texture = (int)prop(calculs->closest.x,\
					(t_dot){wall->p1.x, wall->p2.x},\
					(t_dot){0, wall->texture->w - 1});
	if (x_texture < 0 || x_texture >= wall->texture->w)
	{
		// printf("JVOUS BAISE\n");
		return ;
	}
	
	// printf("Prop : %d\t%d\t%f\n", wall->p1.x, wall->p2.x, calculs->closest.x);
	// printf("Prop : %d\t%d\t%f\n", wall->p1.y, wall->p2.y, calculs->closest.y);

	dy_texture = wall->texture->h / (bas - haut);
	y_texture = 0;
	// haut = (haut < 0) ? 0 : haut;
	// bas = (bas > win->h) ? win->h : bas;
	while (haut++ < bas)
	{
		// printf("Coord texture : %d\t%d\t%d\n", x_texture, (int)y_texture, wall->texture->w);
		pixel = ((Uint32 *)wall->texture->pixels)[(int)y_texture * wall->texture->w + x_texture];
		// pixel = wall->texture->pixels + (int)y_texture * wall->texture->pitch + x_texture * bpp;
		//if (calculs->column > 980)
		SDL_SetRenderDrawColor(win->rend,	(pixel >> 0) & 0xFF,\
											(pixel >> 8) & 0xFF,\
											(pixel >> 16) & 0xFF,\
											(pixel >> 24) & 0x000000FF);
											// calculs->nportals >= 1 ? 100 : 255);
		
		// SDL_SetRenderDrawColor(win->rend,	(pixel & 0xFF000000) >> 24,\
		// 									(pixel & 0x00FF0000) >> 16,\
		// 									(pixel & 0x0000FF00) >> 8,\
		// 									// pixel & 0x000000FF);
		//									calculs->nportals >= 1 ? 100 : 255);
		SDL_RenderDrawPoint(win->rend, calculs->column, win->middle_print + haut);
		y_texture += dy_texture;
	}
}

void	draw(t_win *win, t_map *map, t_player *player)
{
	raycasting(win, player);
	fill_portals(win, player);

	draw_map(win, map);
	//draw_sprite(player, map);
	
	SDL_SetRenderDrawColor(win->rend, 0, 0, 0, 255);
	draw_fps();
}
