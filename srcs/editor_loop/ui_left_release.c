#include "doom_nukem.h"

SDL_Color	get_selected_color(float picker_position)
{
	SDL_Color	color;
	int			pos;

	pos = picker_position / 0.33;

	// printf("pos = %d, position = %f\n", pos, picker_position);
	color = (SDL_Color){0, 0, 0, 255};
	if (pos == 0)
	{
		color.r = 255 - 255 * (picker_position * 4);	
		color.g = 255 * (picker_position * 4);
		color.b = 0;
	}
	else if (pos == 1)
	{
		color.r = 0;	
		color.g = 255 - 255 * ((picker_position - 0.25) * 4);
		color.b = 255 * ((picker_position - 0.25) * 4);
	}
	else if (pos == 2)
	{
		color.r = 255 - 255 * ((picker_position - 0.50) * 4);	
		color.g = 255 * ((picker_position - 0.50) * 4);
		color.b = 0;
	}
	else if (pos == 3)
	{
		color.r = 255 * ((picker_position - 0.75) * 4);	
		color.g = 0;
		color.b = 255 - 255 * ((picker_position - 0.75) * 4);
	}
	return (color);
}

void		resolve_ui_left_release(t_win *win, t_map_editor *map)
{
	t_frame		*f;
	t_button	*b;
	t_sector	*s;
	t_linedef	*l;
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
			{
				add_frame_flags(&win->frames, FRAME_INFO, FRAME_HIDE);
				map->selected_sector = NULL;
			}
			else
			{
				remove_frame_flags(&win->frames, FRAME_INFO, FRAME_HIDE);
				map->selected_sector = s;
			}
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
		}
		else if (win->mouse->x > f->rect.x + (f->rect.w / MAX_SECTORS) * (f->nb_buttons - 1) &&
				win->mouse->x < f->rect.x + (f->rect.w / MAX_SECTORS) * f->nb_buttons)
		{
			add_button_to_frame(&win->selected_frame, new_button((t_frect){1.0 / MAX_SECTORS * f->nb_buttons, 0, 1.0 / MAX_SECTORS, 1}, NULL, 0));
			add_sector(&map->sectors);
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
	else if (f->flags & FRAME_INFO)
	{
		if (win->selected_button)
		{
			b = win->selected_button;
			if (b->flags & BUTTON_COLOR_PICKER)
			{
				map->selected_sector->color.pos = (win->mouse->x - b->rect.x) / (float)b->rect.w;
				if (map->selected_sector)
					map->selected_sector->color.selected_color = get_selected_color(map->selected_sector->color.pos);
			}
			else if (b->flags & BUTTON_EXPORT)
			{
				export_sector(map->selected_sector, "test.sector");
			}
		}
	}
	else if (f->flags & FRAME_L_INFO)
	{
		if (win->selected_button)
		{
			b = win->selected_button;
			if (b->flags & BUTTON_L_TYPE)
			{
				if (b->flags & BUTTON_CLICKED)
				{
					add_frame_flags(&win->frames, FRAME_L_TYPE, FRAME_HIDE);
					b->flags -= BUTTON_CLICKED;
				}
				else
				{
					b->flags |= BUTTON_CLICKED;
					remove_frame_flags(&win->frames, FRAME_L_TYPE, FRAME_HIDE);
				}
			}
		}
	}
	else if (f->flags & FRAME_L_TYPE)
	{
		if (win->selected_button)
		{
			if (map->selected_sector)
			{
				l = map->selected_sector->lines;
				while (l)
				{
					if (l->flags & LINEDEF_SELECTED)
					{
						if (!(l->gflags & win->selected_button->gflags))
							l->gflags = win->selected_button->gflags;
						if (!(win->selected_button->gflags & PORTAL))
							l->id = -1;
					}
					l = l->next;
				}
			}
		}
	}
}