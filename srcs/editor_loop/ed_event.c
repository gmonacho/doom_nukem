#include "doom_nukem.h"
#include "ui_error.h"

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

static SDL_bool	ed_is_mob_selected(t_win *win, const t_map *map, const t_mob *mob)
{
	if (ed_get_line_len(&(t_line){ed_get_map_point(map, win->winui->mouse.pos),
						(t_dot){mob->pos.x, mob->pos.y}}) < mob->width)
		return (SDL_TRUE);
	else
		return (SDL_FALSE);
}

static t_mob	*ed_get_selected_mob(t_win *win, const t_map *map)
{
	t_mob	*m;

	m = map->mob;
	while (m)
	{
		if (ed_is_mob_selected(win, map, m))
			return (m);
		m = m->next;
	}
	return (NULL);
}

static SDL_bool	ed_is_obj_selected(t_win *win, const t_map *map, const t_object *object)
{
	if (ed_get_line_len(&(t_line){ed_get_map_point(map, win->winui->mouse.pos),
						(t_dot){object->pos.x, object->pos.y}}) < object->width)
		return (SDL_TRUE);
	else
		return (SDL_FALSE);
}

static t_object	*ed_get_selected_obj(t_win *win, const t_map *map)
{
	t_object	*obj;

	obj = map->objects;
	while (obj)
	{
		if (ed_is_obj_selected(win, map, obj))
			return (obj);
		obj = obj->next;
	}
	return (NULL);
}

static void		set_int_value(void *argument, char *button_output)
{
	*((int*)argument) = ft_atoi(button_output);
}

static void		set_float_value(void *argument, char *button_output)
{
	*((float*)argument) = ft_atoi(button_output);
}

static void		set_coef_value(void *argument, char *button_output)
{
	*((float*)argument) = ft_atoi(button_output) / 100.f;
}

static void			set_wall_z1(void *argument, char *button_output)
{
	t_poly	*poly;

	poly = argument;
	poly->dots[0].z = ft_atoi(button_output);
	poly->dots[1].z = ft_atoi(button_output);
}

static void			set_wall_z2(void *argument, char *button_output)
{
	t_poly	*poly;

	poly = argument;
	poly->dots[2].z = ft_atoi(button_output);
	poly->dots[3].z = ft_atoi(button_output);
}


static void			set_flat_z(void *argument, char *button_output)
{
	t_poly	*poly;

	poly = argument;
	poly->dots[0].z = ft_atoi(button_output);
	poly->dots[1].z = ft_atoi(button_output);
	poly->dots[2].z = ft_atoi(button_output);
	poly->dots[3].z = ft_atoi(button_output);
}

static void			set_inclined_z1(void *argument, char *button_output)
{
	t_poly	*poly;

	poly = argument;
	poly->dots[0].z = ft_atoi(button_output);
	poly->dots[3].z = ft_atoi(button_output);
}

static void			set_inclined_z2(void *argument, char *button_output)
{
	t_poly	*poly;

	poly = argument;
	poly->dots[1].z = ft_atoi(button_output);
	poly->dots[2].z = ft_atoi(button_output);
}

static void		set_str_value(void *argument, char *button_output)
{
	if (*((char**)argument))
		ft_strdel((char**)argument);
	*((char**)argument) = ft_strdup(button_output);
}

static void	ed_clean_property(t_win *win, int i_start)
{
	t_text_entry_button	*text_entry;
	char				*id;
	char				*tmp;

	while (i_start < 8)
	{
		tmp = ft_itoa(i_start);
		id = ft_strjoin("b_property_", tmp);
		ft_strdel(&tmp);
		text_entry = ui_get_text_entry_button(win->winui, id);
		if (text_entry)
		{
			ft_bzero(text_entry->text, ft_strlen(text_entry->text));
			if (text_entry->name)
				ft_bzero(text_entry->name, ft_strlen(text_entry->name));
		}
		ft_strdel(&id);
		i_start++;
	}
}

static void	ed_set_buttons_wall(t_win *win, t_poly *selected)
{
	t_text_entry_button	*text_entry;
	char				*tmp;

	ui_set_text_entry_function(win->winui, "b_property_1", &set_str_value, &selected->texture_name);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_1");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("texture");
		}
		if (selected->texture_name && ft_strcmp(text_entry->text, selected->texture_name) != 0)
			ft_strcpy(text_entry->text, selected->texture_name);
	}
	ui_set_text_entry_function(win->winui, "b_property_2", &set_wall_z1, selected);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_2");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("z1");
		}
		tmp = ft_itoa(selected->dots[0].z);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ui_set_text_entry_function(win->winui, "b_property_3", &set_wall_z2, selected);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_3");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("z2");
		}
		tmp = ft_itoa(selected->dots[2].z);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ui_set_text_entry_function(win->winui, "b_property_4", &set_coef_value, &selected->light_coef);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_4");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("light %");
		}
		tmp = ft_itoa(selected->light_coef * 100);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ed_clean_property(win, 5);
}

static void	ed_set_buttons_flat(t_win *win, t_poly *selected)
{
	t_text_entry_button	*text_entry;
	char				*tmp;

	ui_set_text_entry_function(win->winui, "b_property_1", &set_str_value, &selected->texture_name);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_1");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("texture");
		}
		if (selected->texture_name && ft_strcmp(text_entry->text, selected->texture_name) != 0)
			ft_strcpy(text_entry->text, selected->texture_name);
	}
	ui_set_text_entry_function(win->winui, "b_property_2", &set_flat_z, selected);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_2");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("z");
		}
		tmp = ft_itoa(selected->dots[0].z);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ui_set_text_entry_function(win->winui, "b_property_3", &set_coef_value, &selected->light_coef);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_3");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("light %");
		}
		tmp = ft_itoa(selected->light_coef * 100);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ed_clean_property(win, 4);
}

static void	ed_set_buttons_inclined(t_win *win, t_poly *selected)
{
	t_text_entry_button	*text_entry;
	char				*tmp;

	ui_set_text_entry_function(win->winui, "b_property_1", &set_str_value, &selected->texture_name);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_1");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("texture");
		}
		if (selected->texture_name && ft_strcmp(text_entry->text, selected->texture_name) != 0)
			ft_strcpy(text_entry->text, selected->texture_name);
	}
	ui_set_text_entry_function(win->winui, "b_property_2", &set_inclined_z1, selected);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_2");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("z1");
		}
		tmp = ft_itoa(selected->dots[0].z);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ui_set_text_entry_function(win->winui, "b_property_3", &set_inclined_z2, selected);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_3");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("z2");
		}
		tmp = ft_itoa(selected->dots[1].z);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ui_set_text_entry_function(win->winui, "b_property_4", &set_coef_value, &selected->light_coef);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_4");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("light %");
		}
		tmp = ft_itoa(selected->light_coef * 100);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ed_clean_property(win, 5);
}

static void	ed_set_buttons_mob(t_win *win, t_mob *selected)
{
	t_text_entry_button	*text_entry;
	char				*tmp;

	ui_set_text_entry_function(win->winui, "b_property_1", &set_str_value, &selected->texture);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_1");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("texture");
		}
		if (selected->texture && ft_strcmp(text_entry->text, selected->texture) != 0)
			ft_strcpy(text_entry->text, selected->texture);
	}
	ui_set_text_entry_function(win->winui, "b_property_2", &set_float_value, &selected->pos.z);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_2");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("z");
		}
		tmp = ft_itoa(selected->pos.z);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ui_set_text_entry_function(win->winui, "b_property_3", &set_int_value, &selected->width);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_3");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("width");
		}
		tmp = ft_itoa(selected->width);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ui_set_text_entry_function(win->winui, "b_property_4", &set_int_value, &selected->height);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_4");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("height");
		}
		tmp = ft_itoa(selected->height);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ui_set_text_entry_function(win->winui, "b_property_5", &set_int_value, &selected->health);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_5");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("health");
		}
		tmp = ft_itoa(selected->health);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ui_set_text_entry_function(win->winui, "b_property_6", &set_int_value, &selected->damage);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_6");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("damage");
		}
		tmp = ft_itoa(selected->damage);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ui_set_text_entry_function(win->winui, "b_property_7", &set_int_value, &selected->vel);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_7");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("velocity");
		}
		tmp = ft_itoa(selected->vel);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
}


static void	ed_set_buttons_object(t_win *win, t_object *selected)
{
	t_text_entry_button	*text_entry;
	char				*tmp;

	ui_set_text_entry_function(win->winui, "b_property_1", &set_str_value, &selected->texture);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_1");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("texture");
		}
		if (selected->texture && ft_strcmp(text_entry->text, selected->texture) != 0)
			ft_strcpy(text_entry->text, selected->texture);
	}
	ui_set_text_entry_function(win->winui, "b_property_2", &set_float_value, &selected->pos.z);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_2");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("z");
		}
		tmp = ft_itoa(selected->pos.z);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ui_set_text_entry_function(win->winui, "b_property_3", &set_int_value, &selected->width);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_3");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("width");
		}
		tmp = ft_itoa(selected->width);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ui_set_text_entry_function(win->winui, "b_property_4", &set_int_value, &selected->height);
	text_entry = ui_get_text_entry_button(win->winui, "b_property_4");
	if (text_entry)
	{
		if (text_entry->name)
		{
			ft_strdel(&text_entry->name);
			text_entry->name = ft_strdup("height");
		}
		tmp = ft_itoa(selected->height);
		ft_strcpy(text_entry->text, tmp);
		ft_strdel(&tmp);
	}
	ed_clean_property(win, 5);
}

static void	ed_selection(t_win *win, t_map *map)
{
	if (win->winui->mouse.clicking & UI_MOUSE_LEFT || win->winui->mouse.clicking & UI_MOUSE_RIGHT)
	{	
		map->editor.select_rect.x = ed_get_map_x(map, win->winui->mouse.pos.x);
		map->editor.select_rect.y = ed_get_map_y(map, win->winui->mouse.pos.y);
		map->editor.select_rect.w = 0;
		map->editor.select_rect.h = 0;
	}
	else if (win->winui->mouse.clicked & UI_MOUSE_LEFT || win->winui->mouse.clicked & UI_MOUSE_RIGHT)
	{
		map->editor.select_rect.w = ed_get_map_x(map, win->winui->mouse.pos.x)
									- map->editor.select_rect.x;
		map->editor.select_rect.h = ed_get_map_y(map, win->winui->mouse.pos.y)
									- map->editor.select_rect.y;
		if ((map->editor.selected_mob = ed_get_selected_mob(win, map)))
		{
			ed_set_buttons_mob(win, map->editor.selected_mob);
			map->editor.selected_obj = NULL;
			map->editor.selected_poly = NULL;
		}
		else if ((map->editor.selected_obj = ed_get_selected_obj(win, map)))
		{
			ed_set_buttons_object(win, map->editor.selected_obj);
			map->editor.selected_mob = NULL;
			map->editor.selected_poly = NULL;
		}
		else if ((map->editor.selected_poly = ed_get_selected_poly(win, map)))
		{
			if (ed_is_wall(map->editor.selected_poly))
				ed_set_buttons_wall(win, map->editor.selected_poly);
			else if (ed_is_flat(map->editor.selected_poly))
				ed_set_buttons_flat(win, map->editor.selected_poly);
			else if (ed_is_inclined(map->editor.selected_poly))
				ed_set_buttons_inclined(win, map->editor.selected_poly);
			map->editor.selected_mob = NULL;
			map->editor.selected_obj = NULL;
		}
		else
		{
			ed_clean_property(win, 1);
			map->editor.selected_mob = NULL;
			map->editor.selected_poly = NULL;
			map->editor.selected_obj = NULL;	
		}
	}
}

static int	ed_add_mob(t_win *win, t_map *map)
{
	t_mob	*mob;

	if (!(mob = ft_memalloc(sizeof(t_mob))))
		return (ui_ret_error("ed_add_mob", "mob allocation failed", 0));
	mob->pos.x = ed_get_map_x(map, win->winui->mouse.pos.x);
	mob->pos.y = ed_get_map_y(map, win->winui->mouse.pos.y);
	mob->pos.z = map->editor.settings.mob.z;
	mob->width = map->editor.settings.mob.width;
	mob->height = map->editor.settings.mob.height;
	mob->health = map->editor.settings.mob.health;
	mob->damage = map->editor.settings.mob.damage;
	mob->vel = map->editor.settings.mob.velocity;
	mob->texture = ft_strdup(map->editor.settings.texture);
	add_existing_mob(&map->mob, mob);
	return (1);
}

static void	ed_place_poly(t_win *win, t_map *map)
{
	SDL_bool	call_place;

	call_place = SDL_FALSE;
	if (win->winui->mouse.releasing & UI_MOUSE_LEFT && !(map->editor.flags & ED_FLAT)
													&& !(map->editor.flags & ED_INCLINED))
	{
		if (map->editor.flags & ED_MOB)
			ed_add_mob(win, map);
		else
		{
			add_existing_poly(&map->polys, map->editor.placing_poly);
			map->editor.placing_poly = NULL;
		}
	}
	else if (win->winui->mouse.clicked & UI_MOUSE_LEFT)
	{
		if (map->editor.flags & ED_WALL)
			ed_place_wall(win, map);
	}
	if (map->editor.flags & ED_FLAT || map->editor.flags & ED_INCLINED)
	{
		if (win->winui->mouse.clicking & UI_MOUSE_LEFT
		|| (win->winui->mouse.releasing & UI_MOUSE_LEFT && map->editor.place_step != 0))
			map->editor.place_step++;
		if (map->editor.place_step == 0
		&& win->winui->mouse.clicking & UI_MOUSE_LEFT)
			call_place = 1;
		else if (map->editor.place_step == 1
		&& win->winui->mouse.clicked & UI_MOUSE_LEFT)
			call_place = 1;
		else if (map->editor.place_step == 2)
			call_place = 1;
		else if (map->editor.place_step == 3)
		{
			add_existing_poly(&map->polys, map->editor.placing_poly);
			map->editor.placing_poly = NULL;
			map->editor.place_step = 0; 
		}
		if (call_place)
		{
			if (map->editor.flags & ED_FLAT)
				ed_place_flat(win, map);
			else if (map->editor.flags & ED_INCLINED)
				ed_place_inclined(win, map);
		}
	}
}

static void	ed_place_item(t_win *win, t_map *map)
{
	t_object	*obj;
	t_dot		pos;

	if (win->winui->mouse.releasing & UI_MOUSE_LEFT)
	{
		if ((obj = (t_object*)ft_memalloc(sizeof(t_object))))
		{
			pos = ed_get_map_point(map, win->winui->mouse.pos);
			if (map->editor.flags & ED_HEAL)
			{
				obj->type = HEAL;
				obj->texture = "healthPotion.png";
			}
			else if (map->editor.flags & ED_SHIELD)
			{
				obj->type = ARMOR;
				obj->texture = "shieldPotion.png";
			}
			else if (map->editor.flags & ED_GRAVITY)
			{
				obj->type = GRAVITY_INV;
				obj->texture = "pq.png";
			}
			else if (map->editor.flags & ED_BULLET)
			{
				obj->type = BULLET;
				obj->texture = "Ammo_box_icon.png";
			}
			else if (map->editor.flags & ED_BOX)
			{
				obj->type = BOX;
				obj->texture = map->editor.settings.texture;
			}
			obj->pos.x = pos.x;
			obj->pos.y = pos.y;
			obj->pos.z = map->editor.settings.object.z;
			obj->width = map->editor.settings.object.width;
			obj->height = map->editor.settings.object.width;
			add_existing_object(&map->objects, obj);
		}
	}
}

static void	ed_action(t_win *win, t_map *map)
{
	// else if (map->editor.flags & ED_PLAYER)
	// 	ed_place_player(win, map);
	if ((win->winui->mouse.clicked & UI_MOUSE_RIGHT || map->editor.flags & ED_SELECTION) && !win->winui->ui.on_mouse_button)
		ed_selection(win, map);
	else if (map->editor.flags & ED_PLAYER  && !win->winui->ui.on_mouse_button)
		ed_place_player(win, map);
	else if ((map->editor.flags & ED_HEAL || map->editor.flags & ED_SHIELD || map->editor.flags & ED_GRAVITY || map->editor.flags & ED_BULLET || map->editor.flags & ED_BOX) && !win->winui->ui.on_mouse_button)
		ed_place_item(win , map);
	else if (map->editor.flags & ED_PLACE && !win->winui->ui.on_mouse_button)
		ed_place_poly(win, map);
	if (map->editor.flags & ED_MODE_CHANGED)
	{
		map->editor.flags ^= ED_MODE_CHANGED;
		if (map->editor.flags & ED_SELECTION || map->editor.flags & ED_PLACE)
			SDL_SetCursor(map->editor.cursor[CURSOR_SELECTING]);
		else
			SDL_SetCursor(map->editor.cursor[CURSOR_DEFAULT]);
	}
}

void	ed_delete_mob(t_mob **mobs, t_mob *mob)
{
	t_mob	*m;
	t_mob	*tmp_prev;
	t_mob	*tmp_next;

	if (mobs && mob)
	{
		tmp_prev = NULL;
		m = *mobs;
		while (m)
		{
			if (m == mob)			
			{
				tmp_next = m->next;
				free(m);
				if (tmp_prev)
					tmp_prev->next = tmp_next;
				else
					*mobs = tmp_next;
				m = tmp_next;
			}
			else
			{
				tmp_prev = m;
				m = m->next;
			}
		}
	}
}

void	ed_delete_obj(t_object **objects, t_object *object)
{
	t_object	*obj;
	t_object	*tmp_prev;
	t_object	*tmp_next;

	if (objects && object)
	{
		tmp_prev = NULL;
		obj = *objects;
		while (obj)
		{
			if (obj == object)
			{
				tmp_next = obj->next;
				free(obj);
				if (tmp_prev)
					tmp_prev->next = tmp_next;
				else
					*objects = tmp_next;
				obj = tmp_next;
			}
			else
			{
				tmp_prev = obj;
				obj = obj->next;
			}
		}
	}
}

int 		ed_event(t_win *win, t_map *map)
{
	const Uint8	*state;
	int			vel;

	state = SDL_GetKeyboardState(NULL);
	if (!win->winui->ui.clicked_button)
	{
		vel = 3;
		if (state[SDL_SCANCODE_LSHIFT])
			vel += 4;
		if (state[SDL_SCANCODE_A])
			map->editor.pos.x -= vel;
		if (state[SDL_SCANCODE_D])
			map->editor.pos.x += vel;
		if (state[SDL_SCANCODE_W])
			map->editor.pos.y -= vel;
		if (state[SDL_SCANCODE_S])
			map->editor.pos.y += vel;
	}
	if (state[SDL_SCANCODE_DELETE] && (map->editor.selected_poly || map->editor.selected_mob || map->editor.selected_obj))
	{
		ed_clean_property(win, 1);
		if (map->editor.selected_poly)
		{
			delete_poly(&map->polys, map->editor.selected_poly);
			map->editor.selected_poly = NULL;
		}
		else if (map->editor.selected_mob)
		{
			ed_delete_mob(&map->mob, map->editor.selected_mob);
			map->editor.selected_mob = NULL;
		}
		else if (map->editor.selected_obj)
		{
			ed_delete_obj(&map->objects, map->editor.selected_obj);
			map->editor.selected_obj = NULL;
		}
	}
	if (state[SDL_SCANCODE_SPACE])
	{
		if (map->editor.flags & ED_DRAW_HELP)
			map->editor.flags ^= ED_DRAW_HELP;
	}
	else
	{
		if (!(map->editor.flags & ED_DRAW_HELP))
			map->editor.flags |= ED_DRAW_HELP;
	}
	if(win->winui->event.type == SDL_MOUSEWHEEL)
	{
		map->editor.unit += win->winui->event.wheel.y * 0.01;
		win->winui->event.wheel.y = 0;
	}
	ed_action(win, map);
	return (1);
}
