#include "doom_nukem.h"

void		update_selected_ui(t_win *win)
{
	t_frame		*f;
	t_button	*b;

	win->selected_frame = NULL;
	win->selected_button = NULL;
	f = win->frames;
	while (f)
	{
		if (!(f->flags & FRAME_HIDE))
		{
			if (is_in_rect(f->rect, (t_dot){win->mouse->x, win->mouse->y}))
			{
				win->selected_frame = f;
				b = f->buttons;
				while (b)
				{
					if (is_in_rect(b->rect, (t_dot){win->mouse->x, win->mouse->y}))
					{
						win->selected_button = b;
						b = NULL;
						f = NULL;
					}
					if (b)
						b = b->next;
				}
			}
		}
		if (f)
			f = f->next;
	}
}

void		update_ui_rect(t_win *win)
{
	t_frame		*f;
	t_button	*b;

	f = win->frames;
	while (f)
	{
		f->rect = (SDL_Rect){win->w * f->ratio.x,
										win->h * f->ratio.y,
										win->w * f->ratio.w,
										win->h * f->ratio.h};
		b = f->buttons;
		while (b)
		{
			b->rect = (SDL_Rect){b->ratio.x * f->rect.w + f->rect.x,
								b->ratio.y * f->rect.h + f->rect.y,
								b->ratio.w * f->rect.w,
								b->ratio.h * f->rect.h};
			b = b->next;
		}
		f = f->next;
	}
}

static char			*str_cconca(const char *s1, char c)
{
	char	*str;
	int		i;

	if (!(str = (char*)ft_memalloc(sizeof(char) * (ft_strlen(s1) + 2))))
		return (ret_null_error("str allocation failed in str_cconca"));
	i = 0;
	if (s1)
	{
		while (*s1)
			str[i++] = *s1++;
	}
	str[i++] = c;
	str[i] = '\0';
	return (str);
}

int				editor_event(t_win *win, t_map_editor *map, SDL_bool *loop)
{
	SDL_Event		event;
	t_linedef		*tmp;
	t_dot			dot;
	//char			*str;
	t_text_entry	*data;


	tmp = NULL;
	SDL_PollEvent(&event);
	mouse_refresh();
	if (event.type == SDL_QUIT)
		*loop = SDL_FALSE;
	else if (event.type == SDL_MOUSEMOTION && !(map->flags & MAP_TEXT_EDITING))
		update_selected_ui(win);
	else if(event.type == SDL_MOUSEWHEEL)
	{
		if (event.wheel.y > 0)
			map_zoom(map, 0.05);
		else if (event.wheel.y < 0)
			map_zoom(map, -0.05);
	}
	else if(event.type == SDL_WINDOWEVENT)
	{
		if(event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			SDL_GetWindowSize(win->ptr, &win->w, &win->h);
			update_ui_rect(win);
		}
	}
	else if (map->flags == MAP_TEXT_EDITING)
	{

		if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE)
		{
			int		int_result;
			char	*char_result;

			data = (t_text_entry*)win->selected_button->data;

			if (data->flags & TEXT_ENTRY_DIGITAL)
			{
				int_result = ft_atoi(win->text_entry);
				if (!(fill_variable(win, map, win->selected_button, &int_result)))
					return (ret_error("fill_variable (int) failed in editor event"));
			}
			else if (data->flags & TEXT_ENTRY_ALPHANUM)
			{
				char_result = win->text_entry;
				if (!(fill_variable(win, map, win->selected_button, &char_result)))
					return (ret_error("fill_variable (char*) failed in editor event"));
			}
			win->text_entry = NULL;
			map->flags -= MAP_TEXT_EDITING;
		}
		if (event.type == SDL_TEXTINPUT)
		{
			data = (t_text_entry*)win->selected_button->data;
			if ((int)ft_strlen(win->text_entry) < data->max_size)
			{
				if ((data->flags & TEXT_ENTRY_DIGITAL && ft_isdigit(event.text.text[0])) ||
					(data->flags & TEXT_ENTRY_ALPHANUM && ft_isalnum(event.text.text[0])))
				{
					if (!(win->text_entry = str_cconca(win->text_entry, event.text.text[0])))
						return (ret_error("ft_strjoin failed in editor_event during TEXTINPUT event"));
					if (!update_text_entry_texture(win, win->selected_button, win->text_entry))
						return (ret_error("update_text_entry_texture failed in editor event"));
				}
			}
		}
	}
	if (win->mouse->button[MOUSE_LEFT].pressing)
	{
		if (win->selected_frame || map->flags & MAP_TEXT_EDITING)
			resolve_ui_left_press(win, map);
		else
		{
			dot = (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit};
			if (!key_pressed(SC_DRAW_FREE))
				is_next_to_linedef(map, &dot, map->unit * NEXT_FACTOR);
			if (!(tmp = new_linedef((t_line){dot, dot}, NULL, LINEDEF_NONE)))
				return (0);
			tmp->gflags = WALL;
			if (map->selected_sector)
				add_linedef(&map->selected_sector->lines, tmp);
			map->flags = map->flags | DRAWING_LINE;
		}
	}
	else if (win->mouse->button[MOUSE_LEFT].releasing)
	{
		if (win->selected_frame && !(map->flags & MAP_TEXT_EDITING))
			resolve_ui_left_release(win, map);
		else
		{
			if (map->flags & DRAWING_LINE)
			{
				dot = (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit};
				if (map->selected_sector)
				{
					if (!key_pressed(SC_DRAW_FREE))
						is_next_to_linedef(map, &dot, map->unit * NEXT_FACTOR);
					map->selected_sector->lines->p2 = dot;
				}
				map->flags -= DRAWING_LINE;
			}
		}
	}
	if (win->mouse->button[MOUSE_RIGHT].pressing)
	{
		if (!win->selected_frame)
		{
			map->rect_util = (SDL_Rect){win->mouse->x, win->mouse->y, 0, 0};
			map->flags = MAP_SELECTING;
		}
	}
	else if (win->mouse->button[MOUSE_RIGHT].releasing)
	{
		if (map->flags & MAP_SELECTING && map->selected_sector)
		{
			selected_linedef(map, map->selected_sector->lines, LINEDEF_SELECTED);
			map->rect_util = (SDL_Rect){};
			map->flags -= MAP_SELECTING;
		}
	}
	if (win->mouse->button[MOUSE_MIDDLE].pressing)
		mouse_drag(win->mouse->x, win->mouse->y, SDL_FALSE);

	if (win->mouse->button[MOUSE_LEFT].pressed)
	{
		if (map->flags & DRAWING_LINE)
		{
			if (map->selected_sector)
			{
				map->selected_sector->lines->p2 = (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit};
				if (!key_pressed(SC_DRAW_FREE))
					is_next_to_linedef(map, &map->selected_sector->lines->p2, map->unit * NEXT_FACTOR);
			}
		}
	}
	if (win->mouse->button[MOUSE_RIGHT].pressed)
	{
		if ( map->selected_sector && map->flags & MAP_SELECTING)
		{
			map->rect_util.w = win->mouse->x - map->rect_util.x;
			map->rect_util.h = win->mouse->y - map->rect_util.y;
			selected_linedef(map, map->selected_sector->lines, LINEDEF_SELECTED);
		}
	}
	if (win->mouse->button[MOUSE_MIDDLE].pressed)
	{
		dot = mouse_drag(win->mouse->x, win->mouse->y, SDL_TRUE);
		map->x += dot.x;
		map->y += dot.y;
		mouse_drag(win->mouse->x, win->mouse->y, SDL_FALSE);
	}
	if (!(map->flags & MAP_TEXT_EDITING))
	{
		if (key_pressed(SDL_SCANCODE_DELETE))
		{
			if (map->selected_sector)
				delete_linedef(&map->selected_sector->lines, LINEDEF_SELECTED);
		}
		if (key_pressed(SDL_SCANCODE_W))
			map->y += 2;
		if (key_pressed(SDL_SCANCODE_S))
			map->y -= 2;
		if (key_pressed(SDL_SCANCODE_A))
			map->x += 2;
		if (key_pressed(SDL_SCANCODE_D))
			map->x -= 2;
	}
	return (1);
}