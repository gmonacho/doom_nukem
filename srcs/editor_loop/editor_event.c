#include "doom_nukem.h"

// void		update_selected_ui(t_win *win)
// {
// 	t_frame		*f;
// 	t_button	*b;
// 	t_button	*tmp_selected;

// 	win->selected_frame = NULL;
// 	tmp_selected = win->selected_button;
// 	win->selected_button = NULL;
// 	f = win->frames;
// 	while (f)
// 	{
// 		if (!(f->flags & FRAME_HIDE))
// 		{
// 			if (is_in_rect(f->rect, (t_dot){win->mouse->x, win->mouse->y}))
// 			{
// 				win->selected_frame = f;
// 				b = f->buttons;
// 				while (b)
// 				{
// 					if (is_in_rect(b->rect, (t_dot){win->mouse->x, win->mouse->y}))
// 					{
// 						win->selected_button = b;
// 						update_button(win, b, BUTTON_STATE_ON_MOUSE);
// 						b = NULL;
// 						f = NULL;
// 					}
// 					if (b)
// 						b = b->next;
// 				}
// 			}
// 		}
// 		if (f)
// 			f = f->next;
// 	}
// 	if (win->selected_button == NULL && tmp_selected)
// 		update_button(win, tmp_selected, BUTTON_STATE_NONE);
// }

// void		update_ui_rect(t_win *win)
// {
// 	t_frame		*f;
// 	t_button	*b;

// 	f = win->frames;
// 	while (f)
// 	{
// 		f->rect = (SDL_Rect){win->w * f->ratio.x,
// 										win->h * f->ratio.y,
// 										win->w * f->ratio.w,
// 										win->h * f->ratio.h};
// 		b = f->buttons;
// 		while (b)
// 		{
// 			b->rect = (SDL_Rect){b->ratio.x * f->rect.w + f->rect.x,
// 								b->ratio.y * f->rect.h + f->rect.y,
// 								b->ratio.w * f->rect.w,
// 								b->ratio.h * f->rect.h};
// 			b = b->next;
// 		}
// 		f = f->next;
// 	}
// }

// static char			*str_cconca(const char *s1, char c)
// {
// 	char	*str;
// 	int		i;

// 	if (!(str = (char*)ft_memalloc(sizeof(char) * (ft_strlen(s1) + 2))))
// 		return (ret_null_error("str allocation failed in str_cconca"));
// 	i = 0;
// 	if (s1)
// 	{
// 		while (*s1)
// 			str[i++] = *s1++;
// 	}
// 	str[i++] = c;
// 	str[i] = '\0';
// 	return (str);
// }

// int				editor_event(t_win *win, t_map_editor *map, SDL_bool *loop)
// {
// 	SDL_Event		event;
// 	t_linedef		*tmp;
// 	t_dot			dot;

// 	//char			*str;
// 	t_text_entry	*data;
// 	int				int_result;
// 	char			*char_result;


// 	tmp = NULL;
// 	SDL_PollEvent(&event);
// 	mouse_refresh();
// 	if (!(map->flags & MAP_TEXT_EDITING))
// 	{
// 		if (key_pressed(SDL_SCANCODE_DELETE))
// 		{
// 			if (map->selected_sector)
// 				delete_linedef(&map->selected_sector->lines, LINEDEF_SELECTED);
// 		}
// 		if (key_pressed(SDL_SCANCODE_W))
// 			map->y += 2;
// 		if (key_pressed(SDL_SCANCODE_S))
// 			map->y -= 2;
// 		if (key_pressed(SDL_SCANCODE_A))
// 			map->x += 2;
// 		if (key_pressed(SDL_SCANCODE_D))
// 			map->x -= 2;
// 	}
// 	if (event.type == SDL_QUIT)
// 		*loop = SDL_FALSE;
// 	else if (event.type == SDL_MOUSEMOTION && !(map->flags & MAP_TEXT_EDITING))
// 	{
// 		if (map->flags & MAP_MOVING_PLAYER)
// 		{
// 			map->player.dpos.x += event.motion.xrel / map->unit;
// 			map->player.dpos.y += event.motion.yrel / map->unit;
// 			update_text_entry_texture(win, get_text_entry_by_name(&win->frames, "x"), ft_itoa(map->player.dpos.x));
// 			update_text_entry_texture(win, get_text_entry_by_name(&win->frames, "y"), ft_itoa(map->player.dpos.y));
// 		}
// 		else
// 			update_selected_ui(win);
// 		dot = (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit};
// 		fill_abscissa_ordinate(map, dot, 3);
// 	}
// 	else if(event.type == SDL_MOUSEWHEEL)
// 	{
// 		if (event.wheel.y > 0)
// 			map_zoom(map, 0.05);
// 		else if (event.wheel.y < 0)
// 			map_zoom(map, -0.05);
// 	}
// 	else if(event.type == SDL_WINDOWEVENT)
// 	{
// 		if(event.window.event == SDL_WINDOWEVENT_RESIZED)
// 		{
// 			SDL_GetWindowSize(win->ptr, &win->w, &win->h);
// 			update_ui_rect(win);
// 		}
// 	}
// 	else if (map->flags == MAP_TEXT_EDITING)
// 	{
// 		if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE)
// 		{
// 			data = (t_text_entry*)win->selected_button->data;
// 			if (data->flags & TEXT_ENTRY_DIGITAL)
// 			{
// 				int_result = ft_atoi(win->text_entry);
// 				if (!(fill_variable(win, map, win->selected_button, &int_result)))
// 					return (ret_error("fill_variable (int) failed in editor event"));
// 			}
// 			else if (data->flags & TEXT_ENTRY_ALPHANUM)
// 			{
// 				char_result = win->text_entry;
// 				if (!(fill_variable(win, map, win->selected_button, char_result)))
// 					return (ret_error("fill_variable (char*) failed in editor event"));
// 				if (win->selected_button->flags & BUTTON_SECTOR_INPUT && (ft_strcmp(win->selected_button->text, "name") == 0))
// 				{
// 					if (!update_frame_button_texture_by_flags(win, BUTTON_SIMPLE, SIMPLE_BUTTON_NAME))
// 						ft_putendl_fd("editor_event : update_frame_button_texture_by_flags failed", 2);
// 				}
// 			}
// 			win->text_entry = NULL;
// 			map->flags -= MAP_TEXT_EDITING;
// 		}
	// if (event.type == SDL_TEXTINPUT)
	// {
	// 	data = (t_text_entry*)win->selected_button->data;
	// 	if ((int)ft_strlen(win->text_entry) < data->max_size)
	// 	{
	// 		if ((data->flags & TEXT_ENTRY_DIGITAL && ft_isdigit(event.text.text[0])) ||
	// 			(data->flags & TEXT_ENTRY_ALPHANUM && ft_isalnum(event.text.text[0])))
	// 		{
	// 			if (!(win->text_entry = str_cconca(win->text_entry, event.text.text[0])))
	// 				return (ret_error("ft_strjoin failed in editor_event during TEXTINPUT event"));
	// 			if (!update_text_entry_texture(win, win->selected_button, win->text_entry))
	// 				return (ret_error("update_text_entry_texture failed in editor event"));
	// 		}
	// 	}
	// }
// 	}
// 	if (win->mouse->button[MOUSE_LEFT].pressing)
// 	{
// 		if (win->selected_frame || map->flags & MAP_TEXT_EDITING)
// 			resolve_ui_left_press(win, map);
// 		else
// 		{
// 			dot.y = (map->ordinate_b) ? map->ordinate.p1.y : (win->mouse->y - map->y) / map->unit;
// 			dot.x = (map->abscissa_b) ? map->abscissa.p1.x : (win->mouse->x - map->x) / map->unit;
// 			if (is_next_point(dot, map->player.dpos, map->player.width))
// 				map->flags |= MAP_MOVING_PLAYER;
// 			else
// 			{
// 				if (!key_pressed(SC_DRAW_FREE))
// 					is_next_to_linedef(map, &dot, NEXT_FACTOR);
// 				if (!(tmp = new_linedef((t_line){dot, dot}, NULL, LINEDEF_NONE)))
// 					return (0);
// 				tmp->gflags = WALL;
// 				if (map->selected_sector)
// 					add_linedef(&map->selected_sector->lines, tmp);
// 				map->flags = map->flags | DRAWING_LINE;
// 			}
// 		}
// 	}
// 	else if (win->mouse->button[MOUSE_LEFT].releasing)
// 	{
// 		if (win->selected_frame && !(map->flags & MAP_TEXT_EDITING))
// 			resolve_ui_left_release(win, map);
// 		else
// 		{
// 			if (map->flags & MAP_MOVING_PLAYER)
// 				map->flags -= MAP_MOVING_PLAYER;
// 			else if (map->flags & DRAWING_LINE)
// 			{
// 				dot = (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit};
// 				if (map->selected_sector)
// 				{
// 					if (!key_pressed(SC_DRAW_FREE))
// 						is_next_to_linedef(map, &dot, NEXT_FACTOR);
// 					map->selected_sector->lines->p2 = dot;
// 				}
// 				map->flags -= DRAWING_LINE;
// 			}
// 		}
// 	}
// 	if (win->mouse->button[MOUSE_RIGHT].pressing)
// 	{
// 		if (!win->selected_frame)
// 		{
// 			map->rect_util = (SDL_Rect){win->mouse->x, win->mouse->y, 0, 0};
// 			map->flags = MAP_SELECTING;
// 		}
// 	}
// 	else if (win->mouse->button[MOUSE_RIGHT].releasing)
// 	{
// 		if (map->flags & MAP_SELECTING && map->selected_sector)
// 		{
// 			selected_linedef(map, map->selected_sector->lines, LINEDEF_SELECTED);
// 			map->rect_util = (SDL_Rect){};
// 			map->flags -= MAP_SELECTING;
// 		}
// 	}
// 	if (win->mouse->button[MOUSE_MIDDLE].pressing)
// 		mouse_drag(win->mouse->x, win->mouse->y, SDL_FALSE);
// 	if (win->mouse->button[MOUSE_LEFT].pressed)
// 	{
// 		if (map->flags & DRAWING_LINE)
// 		{
// 			if (map->selected_sector)
// 			{
// 				dot.y = (map->ordinate_b && !key_pressed(SC_DRAW_FREE)) ? map->ordinate.p1.y : (win->mouse->y - map->y) / map->unit;
// 				dot.x = (map->abscissa_b && !key_pressed(SC_DRAW_FREE)) ? map->abscissa.p1.x : (win->mouse->x - map->x) / map->unit;
// 				map->selected_sector->lines->p2 = dot;
// 				// map->selected_sector->lines->p2 = (t_dot){(win->mouse->x - map->x) / map->unit, (win->mouse->y - map->y) / map->unit};
// 				if (!key_pressed(SC_DRAW_FREE))
// 				{
// 					if(!is_next_to_linedef(map, &map->selected_sector->lines->p2, NEXT_FACTOR))
// 					{
// 						if(is_line_horizontally(map->selected_sector->lines->p1, map->selected_sector->lines->p2, ANG_HOR_VER))
// 							map->selected_sector->lines->p2.y = map->selected_sector->lines->p1.y;
// 						else if (is_line_vertically(map->selected_sector->lines->p1, map->selected_sector->lines->p2, ANG_HOR_VER))
// 							map->selected_sector->lines->p2.x = map->selected_sector->lines->p1.x;
// 					}
// 				}
// 			}
// 		}
// 	}
// 	if (win->mouse->button[MOUSE_RIGHT].pressed)
// 	{
// 		if ( map->selected_sector && map->flags & MAP_SELECTING)
// 		{
// 			map->rect_util.w = win->mouse->x - map->rect_util.x;
// 			map->rect_util.h = win->mouse->y - map->rect_util.y;
// 			selected_linedef(map, map->selected_sector->lines, LINEDEF_SELECTED);
// 		}
// 	}
// 	if (win->mouse->button[MOUSE_MIDDLE].pressed)
// 	{
// 		dot = mouse_drag(win->mouse->x, win->mouse->y, SDL_TRUE);
// 		map->x += dot.x;
// 		map->y += dot.y;
// 		mouse_drag(win->mouse->x, win->mouse->y, SDL_FALSE);
// 	}
// 	return (1);
// }

static void	ed_selection(t_win *win, t_map *map)
{
	if (win->winui->mouse.clicking & UI_MOUSE_LEFT)
	{	
		map->editor.select_rect.x = win->winui->mouse.pos.x / map->editor.unit
									+ map->editor.pos.x;
		map->editor.select_rect.y = win->winui->mouse.pos.y / map->editor.unit
									+ map->editor.pos.y;
		map->editor.select_rect.w = 0;
		map->editor.select_rect.h = 0;
	}
	else if (win->winui->mouse.clicked & UI_MOUSE_LEFT)
	{
		map->editor.select_rect.w = win->winui->mouse.pos.x / map->editor.unit
									+ map->editor.pos.x - map->editor.select_rect.x;
		map->editor.select_rect.h = win->winui->mouse.pos.y / map->editor.unit
									+ map->editor.pos.y - map->editor.select_rect.y;
		map->editor.selected_poly = ed_get_selected_poly(map);
	}
	// else if (win->winui->mouse.releasing & UI_MOUSE_LEFT)
	// {
	// }
}

static void	ed_action(t_win *win, t_map *map)
{
	if (map->editor.flags & ED_SELECTION)
		ed_selection(win, map);
	if (map->editor.flags & ED_MODE_CHANGED)
		map->editor.flags ^= ED_MODE_CHANGED;
}

int 		ed_event(t_win *win, t_map *map)
{
	const Uint8	*state;

	state = SDL_GetKeyboardState(NULL);
	if (win)
	{
		if (state[SDL_SCANCODE_A])
			map->editor.pos.x -= 1;
	}
	if (state[SDL_SCANCODE_D])
		map->editor.pos.x += 1;
	if (state[SDL_SCANCODE_W])
		map->editor.pos.y -= 1;
	if (state[SDL_SCANCODE_S])
		map->editor.pos.y += 1;
	if(win->winui->event.type == SDL_MOUSEWHEEL)
	{
		map->editor.unit += win->winui->event.wheel.y * 0.01;
		win->winui->event.wheel.y = 0;
	}
	ed_action(win, map);
	return (1);
}
