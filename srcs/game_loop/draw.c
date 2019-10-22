#include "doom_nukem.h"

/*
**	Il ne faut pas allonger l'image en largeur mais la repete
**	En hauteur aussi
**
**	player->dir_up est le milieu y du mur sur lecran,
**	On +/- le y du perso et le snick.
**	La vision du perso retrecie en haut et en bas (On diminue HEIGHT_WALL)
**	On multiplie car prop() joue un role de facteur d'agrandissement
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

void			print_column(t_win *win, t_linedef *wall, t_player *player, t_calculs *calculs)
{
	Uint32			pixel;
	int				x_texture;
	double			y_texture;
	// int				n_texture;
	double			dy_texture;
	int				n_texture_y;
	int				i;
	int				n_pixels;

	if (!wall)
	{
		// SDL_SetRenderDrawColor(win->rend, 0, 0, 0, 255);
		// i = -1;
		// while (++i < win->h)
		// 	SDL_RenderDrawPoint(win->rend, calculs->column, i);
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

	// printf("Dir up : %fpi\n", player->dir_up / M_PI);
	win->middle_print = fprop(player->dir_up,\
							(t_fdot){-player->fov_up / 2, player->fov_up / 2},\
							(t_fdot){win->h, 0});
	// printf("win->middle_print : %f\n", win->middle_print);
	win->middle_print -= (player->shift ? player->height / 2 : 0);

	calculs->up_wall = (player->dir_up < win->h / 2) ?\
						prop(player->dir_up, (t_dot){0, win->h / 2}, (t_dot){3 * HEIGHT_WALL / 4, HEIGHT_WALL}) :\
						prop(player->dir_up, (t_dot){win->h / 2, win->h}, (t_dot){HEIGHT_WALL, 3 * HEIGHT_WALL / 4});
	calculs->low_wall = calculs->up_wall;

	calculs->up_wall *= -(wall->sector->ceil_height - (player->height + player->z + player->sector->floor_height)) / player->lenRay;
	calculs->low_wall *= (player->height + player->z + (player->sector->floor_height - wall->sector->floor_height)) / player->lenRay;
	// printf(" : %f\n", calculs->up_wall);
	// printf("Lenray : %f\n", player->lenRay);
	// printf("Column : %d\haut : %f\tBas : %f\n", calculs->column, calculs->up_wall, bas);
	// printf("Calcul haut : %d\n", wall->sector->ceil_height - (player->height + player->sector->floor_height));
	// printf("Bas : %d\n", player->height + (player->sector->floor_height - wall->sector->floor_height));
	// SDL_SetRenderDrawColor(win->rend, 0x40, 0xDD, 0x40, 255);
	// draw_column(win, calculs->column, win->middle_print - calculs->up_wall,\
	// 									win->middle_print + bas);

	// if (wall->p1.x == wall->p2.x)
	// 	x_texture = (int)prop(calculs->closest.y,\
	// 				(t_dot){wall->p1.y, wall->p2.y},\
	// 				(t_dot){0, wall->texture->w - 1});
	// else
	// 	x_texture = (int)prop(calculs->closest.x,\
	// 				(t_dot){wall->p1.x, wall->p2.x},\
	// 				(t_dot){0, wall->texture->w - 1});

	//Ratio : 1 pixel = 1u^2 dans le plan
	int ratio = 1;
	if (wall->p1.x == wall->p2.x)
	{
		// n_texture_x = (int)ft_abs(calculs->closest.y - wall->p1.y) / wall->texture->w;
		// x_texture = (int)ft_abs(calculs->closest.y - wall->p1.y) % (int)(wall->texture->w / ratio);
		// printf("Inter : %f\t%d\n", calculs->closest.y, wall->p1.y);
	}
	else
	{
		// x_texture = (int)ft_abs(calculs->closest.x - wall->p1.x) / (int)(wall->texture->w / ratio);
		// printf("Inter : %f\t%d\n", calculs->closest.x, wall->p1.x);
	}
	x_texture = (int)fdist((t_fdot){wall->p1.x, wall->p1.y}, calculs->closest) %\
				(int)(wall->texture->w / ratio);
	// printf("\nDist : %f\t%d\n", fdist((t_fdot){wall->p1.x, wall->p1.y}, calculs->closest), x_texture);
	// printf("Size img : %d/%d\n", wall->texture->w, wall->texture->h);
	// printf("Haut/bas : %f\t%f\n", haut, bas);

	// if (x_texture < 0 || x_texture >= wall->texture->w)
	// {
	// 	printf("JVOUS BAISE : %f\t%d\t%d\n", calculs->closest.y, wall->p1.y, wall->p2.y);
	// 	return ;
	// }

	// for (int p = 0; p < wall->texture->w; p++)
	// 	for (int p2 = 0; p2 < wall->texture->h; p2++)
	// 	{
	// 		printf("x/y : %d/%d\t", p, p2);
	// 		printf("Color : %d\n", (int)(((int *)wall->texture->pixels)[p2 * wall->texture->w + p]));
	// 	}

	n_texture_y = 0;
	n_pixels = fabs(calculs->low_wall) + fabs(calculs->up_wall);
	// dy_texture = (float)wall->texture->h / n_pixels;
	dy_texture = (float)wall->sector->height / n_pixels;
	// printf("Height : %d\n", wall->sector->height);
	// printf("\nDy : %f\n", dy_texture);
	i = -1;
	// printf("wall->texture ptr = %p\n", wall->texture);
	while (++i < n_pixels)
	{
		y_texture = (int)(i * dy_texture) % (int)(wall->texture->h / ratio);
		// printf("Coord : %d\t%d\n", x_texture, y_texture);
		// printf("Coord : %d\t%f\t%d\t%d\n", x_texture, y_texture, wall->texture->w, wall->texture->h);
		// pixel = ((Uint32 *)wall->texture->pixels)[(int)y_texture * wall->texture->w + x_texture];
		pixel = ((Uint32 *)wall->texture->pixels)[(int)y_texture * wall->texture->w + (int)x_texture];
		SDL_SetRenderDrawColor(win->rend,	(pixel >> 16) & 0xFF,\
											(pixel >> 8) & 0xFF,\
											(pixel >> 0) & 0xFF,\
											(pixel >> 24) & 0xFF);
											// calculs->nportals >= 1 ? 100 : 255);

		SDL_RenderDrawPoint(win->rend, calculs->column, win->middle_print + calculs->up_wall + i);
		//y_texture += 1 + (y_texture + 1 == wall->texture->h ? -wall->texture->h : 0);
		// y_texture += dy_texture;
	}
	// n_texture_y = wall->sector->height / wall->texture->h;
	// dy_texture = (n_texture_y * wall->texture->h + wall->sector->height % wall->texture->h) / (bas - haut);
	// y_texture = 0;
	// while (haut++ < bas)
	// {
	// 	if (y_texture >= wall->texture->h)
	// 		y_texture -= wall->texture->h;
	// 	// printf("Coord : %d\t%d\t%d\t%d\n", x_texture, y_texture, wall->texture->w, wall->texture->h);
	// 	// pixel = ((Uint32 *)wall->texture->pixels)[(int)y_texture * wall->texture->w + x_texture];
	// 	pixel = ((Uint32 *)wall->texture->pixels)[(int)y_texture * wall->texture->w + x_texture];
	// 	SDL_SetRenderDrawColor(win->rend,	(pixel >> 16) & 0xFF,\
	// 										(pixel >> 8) & 0xFF,\
	// 										(pixel >> 0) & 0xFF,\
	// 										(pixel >> 24) & 0xFF);
	// 										// calculs->nportals >= 1 ? 100 : 255);

	// 	SDL_RenderDrawPoint(win->rend, calculs->column, win->middle_print + haut);
	// 	//y_texture += 1 + (y_texture + 1 == wall->texture->h ? -wall->texture->h : 0);
	// 	y_texture += dy_texture;
	// }
}

void	draw(t_win *win, t_map *map, t_player *player)
{
	raycasting_3d(win, player);
	// raycasting(win, player);
	fill_portals(win, player);

	draw_map(win, map);
	//draw_sprite(player, map);
	
	SDL_SetRenderDrawColor(win->rend, 0, 0, 0, 255);
	draw_fps();
}



/*

if (wall->p1.x == wall->p2.x)
		x_texture = (int)prop(calculs->closest.y,\
					(t_dot){wall->p1.y, wall->p2.y},\
					(t_dot){0, wall->texture->w - 1});
	else
		x_texture = (int)prop(calculs->closest.x,\
					(t_dot){wall->p1.x, wall->p2.x},\
					(t_dot){0, wall->texture->w - 1});

	//Ratio : 1 pixel = 1u^2 dans le plan
	// if (wall->p1.x == wall->p2.x)
	// {
	// 	n_texture = ft_abs(calculs->closest.y - wall->p1.y) / wall->texture->w;
	// 	x_texture = ft_abs(calculs->closest.y - wall->p1.y) % wall->texture->w;
	// }
	// else
	// {
	// 	n_texture = ft_abs(calculs->closest.x - wall->p1.x) / wall->texture->w;
	// 	x_texture = ft_abs(calculs->closest.x - wall->p1.x) % wall->texture->w;
	// }

	if (x_texture < 0 || x_texture >= wall->texture->w)	//Pansement MOCHE A ENLEVER mais necessaire
	{
		printf("JVOUS BAISE : %f\t%d\t%d\n", calculs->closest.y, wall->p1.y, wall->p2.y);
		return ;
	}


	dy_texture = wall->texture->h / (bas - haut);
	y_texture = 0;
	while (haut++ < bas && y_texture < wall->texture->h)
	{
		// printf("Coord texture : %d\t%d\t%d\n", x_texture, (int)y_texture, wall->texture->h);
		pixel = ((Uint32 *)wall->texture->pixels)[(int)y_texture * wall->texture->w + x_texture];
		// pixel = wall->texture->pixels + (int)y_texture * wall->texture->pitch + x_texture * bpp;
		SDL_SetRenderDrawColor(win->rend,	(pixel >> 16) & 0xFF,\
											(pixel >> 8) & 0xFF,\
											(pixel >> 0) & 0xFF,\
											(pixel >> 24) & 0xFF);
											// calculs->nportals >= 1 ? 100 : 255);
		
		// SDL_SetRenderDrawColor(win->rend,	(pixel & 0xFF000000) >> 24,\
		// 									(pixel & 0x00FF0000) >> 16,\
		// 									(pixel & 0x0000FF00) >> 8,\
		// 									// pixel & 0x000000FF);
		//									calculs->nportals >= 1 ? 100 : 255);
		SDL_RenderDrawPoint(win->rend, calculs->column, win->middle_print + haut);
		y_texture += dy_texture;
	}
*/