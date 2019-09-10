#include "doom_nukem.h"

void		resolve_ui_left_release(t_win *win, t_map_editor *map)
{
	t_frame			*f;
	t_frame			*info_sector;
	t_button		*b_sector;
	t_button		*b;
	t_sector		*s;
	t_linedef		*l;
	t_text_entry	*data;
	int				i;
	int				i_sector;
	int				nb_sectors;

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
				info_sector = get_frame(&win->frames, FRAME_INFO);
				if (info_sector)
				{
					b_sector = info_sector->buttons;
					while (b_sector)
					{
						if (b_sector->data && b_sector->flags & BUTTON_TEXT_ENTRY)
						{
							data = (t_text_entry*)b_sector->data;
							if (data->flags & TEXT_ENTRY_SECTOR_NAME)
								data->variable = map->selected_sector->name;
							else if (data->flags & TEXT_ENTRY_SECTOR_FLOOR)
								data->variable = &map->selected_sector->floor_height;
							else if (data->flags & TEXT_ENTRY_SECTOR_CEIL)
								data->variable = &map->selected_sector->ceil_height;
							if (data->variable)
								printf("data->variable = %s\n", data->variable);
							if (data->flags & TEXT_ENTRY_ALPHANUM)
								update_text_entry_texture(win, b_sector, data->variable);
							else if (data->flags & TEXT_ENTRY_DIGITAL)
								update_text_entry_texture(win, b_sector, ft_itoa(*(int*)data->variable));
						}
						b_sector = b_sector->next;
					}
				}
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
			if (b->flags & BUTTON_EXPORT)
			{
				export_sector(&map->player, map->selected_sector, map->selected_sector->name);
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
			else if (b->flags & BUTTON_LINEDEF_SIDE)
			{
				if (map->selected_sector)
				{
					l = map->selected_sector->lines;
					while (l)
					{
						if (l->flags & LINEDEF_SELECTED)
						{
							if (l->side == SIDE_LEFT)
								l->side = SIDE_RIGHT;
							else
								l->side = SIDE_LEFT;
						}
						l = l->next;
					}
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
	else if (f->flags & FRAME_MAP)
	{
		if (win->selected_button)
		{
			b = win->selected_button;
			if (b->flags & BUTTON_MAP_EXPORT)
			{
				printf("map->name = %s\n", map->name);
				export_map(map, map->name);
			}
		}
	}
}