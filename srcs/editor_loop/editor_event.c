#include "doom_nukem.h"

static void		update_selected_ui(t_win *win)
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

static void		update_ui_rect(t_win *win)
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

static char			*str_conca(const char *s1, char c)
{
	char	*str;
	int		i;

	if (!(str = (char*)ft_memalloc(sizeof(char) * (ft_strlen(s1) + 2))))
		return (ret_null_error("str allocation failed in str_conca"));
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
	SDL_Event	event;
	t_linedef	*tmp;
	t_dot		dot;
	char		*str;


	tmp = NULL;
	SDL_PollEvent(&event);
	mouse_refresh();
	if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
		*loop = SDL_FALSE;
	else if (event.type == SDL_MOUSEMOTION)
	{
		update_selected_ui(win);
	}
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
	else if (event.type == SDL_TEXTINPUT && map->flags == MAP_TEXT_EDITING)
	{
		str = map->selected_sector->name;
		// printf("event.text = %s\n", event.text.text);
		if (!(map->selected_sector->name = str_conca(map->selected_sector->name, event.text.text[0])))
			return (ret_error("ft_strjoin failed in editor_event during TEXTINPUT event"));
		if (!(win->text_outpout = generate_text(win->rend, win->font, map->selected_sector->name, (SDL_Color){200, 200, 200, 255})))
			return (ret_error("text generation failed in editor_event during TEXTINPUT event"));
		
		ft_strdel(&str);
		printf("text = %s\n", map->selected_sector->name);
	}
	if (win->mouse->button[MOUSE_LEFT].pressing)
	{
		if (win->selected_frame)
			resolve_ui_left_press(win, map);
		else
		{
			dot = (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit};
			if (!key_pressed(SC_DRAW_FREE))
				is_next_to_linedef(map, &dot, map->unit * NEXT_FACTOR);
			if (!(tmp = new_linedef((t_line){dot, dot}, NULL, LINEDEF_NONE)))
				return (0);
			if (map->selected_sector)
				add_linedef(&map->selected_sector->lines, tmp);
			map->flags = map->flags | DRAWING_LINE;
		}
	}
	else if (win->mouse->button[MOUSE_LEFT].releasing)
	{
		if (win->selected_frame)
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