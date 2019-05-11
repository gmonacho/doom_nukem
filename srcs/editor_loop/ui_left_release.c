#include "doom_nukem.h"

void		resolve_ui_left_release(t_win *win, t_map_editor *map)
{
	t_frame		*f;
	t_button	*b;
	t_sector	*s;
	int			i;
	int			i_sector;
	int			nb_sectors;

	nb_sectors = get_nb_sectors(map->sectors);
	f = win->selected_frame;
	if (f->flags & FRAME_SECTORS)
	{
		if (win->mouse->x < f->rect.x + (f->rect.w / MAX_SECTORS) * (f->nb_buttons - 1))
		{
			i_sector = (win->mouse->x - f->rect.x) / (f->rect.w / MAX_SECTORS);
			s = map->sectors;
			i = 0;
			while (i < nb_sectors - i_sector - 1)
			{
				s = s->next;
				i++;
			}
			if (map->selected_sector == s)
				map->selected_sector = NULL;
			else
				map->selected_sector = s;
			i = 0;
			b = f->buttons;
			b->texture = win->sectors_texture[MAX_SECTORS];
			b = b->next;
			while (b)
			{
				if (is_in_rect(b->rect, (t_dot){win->mouse->x, win->mouse->y}) && map->selected_sector)
					b->texture = win->sectors_texture_selected[nb_sectors - i - 1];
				else
					b->texture = win->sectors_texture[nb_sectors - i - 1];
				b = b->next;
				i++;
			}
			printf("map->selected_sector = %p\n", map->selected_sector);
		}
		else if (win->mouse->x > f->rect.x + (f->rect.w / MAX_SECTORS) * (f->nb_buttons - 1) &&
				win->mouse->x < f->rect.x + (f->rect.w / MAX_SECTORS) * f->nb_buttons)
		{
			add_button_to_frame(&win->selected_frame, new_button((t_frect){1.0 / MAX_SECTORS * f->nb_buttons, 0, 1.0 / MAX_SECTORS, 1}, NULL, 0));
			add_sector(&map->sectors, new_sector());
			b = f->buttons;
			i = 0;
			b->texture = win->sectors_texture[MAX_SECTORS];
			b = b->next;
			while (b)
			{
				b->texture = win->sectors_texture[nb_sectors - i];
				b = b->next;
				i++;
			}
		}
	}
}