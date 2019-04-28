#include "loop.h"
#include "display.h"
#include "event.h"
#include "data.h"
#include "ret_error.h"
#include "event.h"
#include "physics.h"

void	draw(t_win *win, t_map *map, t_player *player)
{
	t_sector	*sector;
	t_linedef	*line;

	sector = map->sectors;
	while (sector)
	{
		line = sector->lines;
		while (line)
		{
			draw_line(win, line->d1, line->d2);
			line = line->next;
		}
		sector = sector->next;
	}

	SDL_SetRenderDrawColor(win->rend, 0x20, 0xDD, 0x20, 255);
	SDL_RenderDrawPoint(win->rend, player->pos.x, player->pos.y);
}