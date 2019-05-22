#include "doom_nukem.h"

void	draw(t_win *win, t_map *map, t_player *player)
{
	t_sector	*sector;
	t_linedef	*line;
	t_circle	perso;

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

	perso = (t_circle){player->pos.x, player->pos.y, player->hitbox};
	draw_circle(win, perso);
	draw_line(win, player->pos, (t_dot){player->pos.x + 30 * player->vel.x,\
										player->pos.y + 30 * player->vel.y});
	//draw_fps();
}
