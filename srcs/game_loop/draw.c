#include "doom_nukem.h"

void	draw(t_win *win, t_map *map, t_player *player)
{
	t_sector	*sector;
	t_linedef	*line;

	//printf("Debut print\n");
	raycasting(win, player);
 
	SDL_SetRenderDrawColor(win->rend, 0, 0, 0, 255);
	sector = map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			draw_line(win, line->p1, line->p2);
			line = line->next;
		}
		sector = sector->next;
	}

	SDL_SetRenderDrawColor(win->rend, 0, 0, 0, 255);
	//SDL_RenderDrawPoint(win->rend, (int)player->pos.x, (int)player->pos.y);

	//printf("Pos : %f\t%f\n\n", player->pos.x, player->pos.y);
	draw_circle(win, (t_circle){player->pos.x, player->pos.y, player->width / 2});
	draw_line(win, (t_dot){player->pos.x, player->pos.y}, (t_dot){player->pos.x + 30 * cos(player->dir),\
																player->pos.y + 30 * sin(player->dir)});
	
	//map = NULL;
	draw_fps();
}
