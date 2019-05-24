#include "doom_nukem.h"

void	draw(t_win *win, t_map *map, t_player *player)
{
	t_sector	*sector;
	t_linedef	*line;

	//printf("Debut print\n");
	clear_rend(win->rend, 0, 0, 0);

	SDL_SetRenderDrawColor(win->rend, 0xFF, 0xFF, 0xFF, 255);
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

	SDL_SetRenderDrawColor(win->rend, 0x20, 0xDD, 0x20, 255);
	//SDL_RenderDrawPoint(win->rend, (int)player->pos.x, (int)player->pos.y);

	draw_circle(win, (t_circle){player->pos.x, player->pos.y, player->width / 2});
	draw_line(win, (t_dot){player->pos.x, player->pos.y}, (t_dot){player->pos.x + 30 * cos(player->dir),\
										player->pos.y + 30 * -sin(player->dir)});
	//draw_fps();
}
