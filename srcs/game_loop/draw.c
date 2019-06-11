#include "doom_nukem.h"

static void	draw_map(t_win *win, t_map *map)
{
	t_sector	*sector;
	t_linedef	*line;

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
}

/*void	draw_sprite(t_player *player, t_object *object)
{
	while (object)
	{

		object = object->next;
	}
}*/

void	draw(t_win *win, t_map *map, t_player *player)
{
	raycasting(win, player);

	draw_map(win, map);
	//draw_sprite(player, map);
	
	SDL_SetRenderDrawColor(win->rend, 0, 0, 0, 255);
	draw_circle(win, (t_circle){player->pos.x, player->pos.y, player->width / 2});
	draw_line(win, (t_dot){player->pos.x, player->pos.y}, (t_dot){player->pos.x + 30 * cos(player->dir),\
																player->pos.y + 30 * sin(player->dir)});
	draw_fps();
}
