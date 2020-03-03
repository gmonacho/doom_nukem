#include "doom_nukem.h"
#include "ui.h"
#include "ui_draw.h"
#include "ui_error.h"

static void	editor_menu_quit(t_win *win, t_map *map, Uint32 ms)
{
	Mix_FadeOutMusic(ms);
	ui_free_ui(&win->winui->ui);
	SDL_SetCursor(map->editor.cursor[CURSOR_DEFAULT]);
	SDL_Delay(ms);
}

static void		editor_menu_disp(t_win *win, t_map *map)
{
	map->editor.min_pos_z = ed_get_z_min(map->polys);
	map->editor.max_pos_z = ed_get_z_max(map->polys);
	ui_set_draw_color(win->rend, &(SDL_Color){30, 30, 30, 255});
	ui_clear_win(win->winui);
	ed_display(win, map);
	ui_display_frames(win->winui, win->winui->ui.frames);
	ui_draw_rend(win->winui);
	ui_poll_event(&win->winui->event);
	ui_update_ui(win->winui);
}

static void		set_int_value(void *argument, char *button_output)
{
	*((int*)argument) = ft_atoi(button_output);
}

static void		set_str_value(void *argument, char *button_output)
{
	if (*((char**)argument))
		ft_strdel((char**)argument);
	*((char**)argument) = ft_strdup(button_output);
}

static void		set_editor_flags(void *argument)
{
	t_arg_menu	*arg_menu;

	arg_menu = (t_arg_menu*)argument;

	if (ED_ALL_TYPES & arg_menu->value)
	{
		if (*(arg_menu->loop) & arg_menu->value)
		{
			*(arg_menu->loop) ^= arg_menu->value;
			*(arg_menu->loop) ^= ED_PLACE;
		}
		else
		{
			if (*(arg_menu->loop) & ED_WALL)
				*(arg_menu->loop) ^= ED_WALL;
			else if (*(arg_menu->loop) & ED_FLAT)
				*(arg_menu->loop) ^= ED_FLAT;
			else if (*(arg_menu->loop) & ED_INCLINED)
				*(arg_menu->loop) ^= ED_INCLINED;
			else if (*(arg_menu->loop) & ED_HEAL)
				*(arg_menu->loop) ^= ED_HEAL;
			else if (*(arg_menu->loop) & ED_SHIELD)
				*(arg_menu->loop) ^= ED_SHIELD;
			else if (*(arg_menu->loop) & ED_GRAVITY)
				*(arg_menu->loop) ^= ED_GRAVITY;
			else if (*(arg_menu->loop) & ED_BULLET)
				*(arg_menu->loop) ^= ED_BULLET;
			else if (*(arg_menu->loop) & ED_BOX)
				*(arg_menu->loop) ^= ED_BOX;
			else if (*(arg_menu->loop) & ED_PLAYER)
				*(arg_menu->loop) ^= ED_PLAYER;
			if (*(arg_menu->loop) & ED_SELECTION)
				*(arg_menu->loop) ^= ED_SELECTION;
			*(arg_menu->loop) |= arg_menu->value;
			*(arg_menu->loop) |= ED_PLACE;
		}
	}
	else
	{
		if (*(arg_menu->loop) & arg_menu->value)
			*(arg_menu->loop) ^= arg_menu->value;
		else
		{
			*(arg_menu->loop) |= arg_menu->value;
			if (*(arg_menu->loop) & ED_WALL)
				*(arg_menu->loop) ^= ED_WALL;
			else if (*(arg_menu->loop) & ED_FLAT)
				*(arg_menu->loop) ^= ED_FLAT;
			else if (*(arg_menu->loop) & ED_INCLINED)
				*(arg_menu->loop) ^= ED_INCLINED;
			if (*(arg_menu->loop) & ED_PLACE)
				*(arg_menu->loop) ^= ED_PLACE;
		}
	}
	*(arg_menu->loop) |= ED_MODE_CHANGED;
}

static void		set_editor_calc(void *argument)
{
	t_arg_menu	*arg;

	arg = (t_arg_menu*)argument;
	*(arg->loop) = arg->value;
}

static void		set_menu_button_function(t_win *win, t_map *map)
{
	ui_set_simple_button_function(win->winui,
								"b_select",
								&set_editor_flags,
								&map->editor.arg_menu_tab[0]);
	ui_set_simple_button_function(win->winui,
								"b_wall",
								&set_editor_flags,
								&map->editor.arg_menu_tab[1]);
	ui_set_simple_button_function(win->winui,
								"b_flat",
								&set_editor_flags,
								&map->editor.arg_menu_tab[2]);
	ui_set_simple_button_function(win->winui,
								"b_inclined",
								&set_editor_flags,
								&map->editor.arg_menu_tab[3]);
	ui_set_simple_button_function(win->winui,
								"b_player",
								&set_editor_flags,
								&map->editor.arg_menu_tab[4]);
	ui_set_simple_button_function(win->winui,
								"b_calc_normal",
								&set_editor_calc,
								&map->editor.arg_menu_tab[5]);
	ui_set_simple_button_function(win->winui,
								"b_calc_z",
								&set_editor_calc,
								&map->editor.arg_menu_tab[6]);
	ui_set_simple_button_function(win->winui,
								"b_mob",
								&set_editor_flags,
								&map->editor.arg_menu_tab[7]);
	ui_set_simple_button_function(win->winui,
								"b_heal",
								&set_editor_flags,
								&map->editor.arg_menu_tab[8]);
	ui_set_simple_button_function(win->winui,
								"b_shield",
								&set_editor_flags,
								&map->editor.arg_menu_tab[9]);
	ui_set_simple_button_function(win->winui,
								"b_gravity",
								&set_editor_flags,
								&map->editor.arg_menu_tab[10]);
	ui_set_simple_button_function(win->winui,
								"b_bullet",
								&set_editor_flags,
								&map->editor.arg_menu_tab[11]);
	ui_set_simple_button_function(win->winui,
								"b_box",
								&set_editor_flags,
								&map->editor.arg_menu_tab[12]);
	ui_set_simple_button_function(win->winui,
								"b_export",
								&ed_export,
								&map->editor.export);
	ui_set_simple_button_function(win->winui,
								"b_delete",
								&ed_delete_map,
								map);
	ui_set_text_entry_function(win->winui, "b_z_min", &set_int_value, &map->editor.z_min);
	ui_set_text_entry_function(win->winui, "b_z_max", &set_int_value, &map->editor.z_max);
	ui_set_text_entry_function(win->winui, "b_export_path", &set_str_value, &map->editor.export.path);
}

int		init_editor_menu(t_win *win, t_map *map)
{
	// if (Mix_PlayMusic(win->music.editor_music, -1) == -1)
	// 	ui_ret_error("init_editor_menu", "impossible to play menu_music", 0);
	if (!(win->winui->ui.button_font = ui_load_font("TTF/arial.ttf", 100)))
		return (ui_ret_error("init_editor_menu", "ui_load_font failed", 0));
	if (!ui_load("interfaces/editor_interface", win->winui))
		return (ui_ret_error("init_editor_menu", "ui_load failed", 0));
	set_menu_button_function(win, map);
	SDL_SetRelativeMouseMode(SDL_FALSE);
	return (1);
}

// static SDL_bool		ed_is_mob_poly(const t_map *map, const t_poly *poly)
// {
// 	const t_mob	*m;

// 	m = map->mob;
// 	while (m)
// 	{
// 		if (m->poly == poly)
// 			return (SDL_TRUE);
// 		m = m->next;
// 	}
// 	return (SDL_FALSE);
// }

// static void		ed_delete_mob_polys(t_map *map)
// {
// 	t_poly	*p;
// 	t_poly	*next;
// 	t_poly	*previous;

// 	if (map)
// 	{
// 		previous = NULL;
// 		p = map->polys;
// 		while (p)
// 		{
// 			next = p->next;
// 			if (ed_is_mob_poly(map, p))
// 			{
// 				next = p->next;
// 				free(p);
// 				if (previous)
// 					previous->next = next;
// 				else
// 					map->polys = next;
// 				p = next;
// 			}
// 			else
// 			{
// 				previous = p;
// 				p = p->next;
// 			}
// 		}
// 	}
// }


// static SDL_bool		ed_is_object_poly(const t_map *map, const t_poly *poly)
// {
// 	const t_object	*obj;

// 	obj = map->objects;
// 	while (obj)
// 	{
// 		if (obj->poly == poly)
// 			return (SDL_TRUE);
// 		obj = obj->next;
// 	}
// 	return (SDL_FALSE);
// }

// static void		ed_delete_object_polys(t_map *map)
// {
// 	t_poly	*p;
// 	t_poly	*next;
// 	t_poly	*previous;

// 	if (map)
// 	{
// 		previous = NULL;
// 		p = map->polys;
// 		while (p)
// 		{
// 			next = p->next;
// 			if (ed_is_object_poly(map, p))
// 			{
// 				next = p->next;
// 				free(p);
// 				if (previous)
// 					previous->next = next;
// 				else
// 					map->polys = next;
// 				p = next;
// 			}
// 			else
// 			{
// 				previous = p;
// 				p = p->next;
// 			}
// 		}
// 	}
// }

int				editor_loop(t_win *win, t_map *map)
{
	SDL_bool			loop;

	map->editor.pos = (t_dot){0, 0};
	map->editor.size = (t_dot){0, 0};
	map->editor.unit = 1;
	map->editor.z_min = 0;
	map->editor.z_max = 100;
	map->editor.settings.wall.min = 0;
	map->editor.settings.wall.max = 100;
	map->editor.settings.mob.z = 5;
	map->editor.settings.mob.width = 30;
	map->editor.settings.mob.height = 30;
	map->editor.settings.mob.damage = 1;
	map->editor.settings.mob.health = 50;
	map->editor.settings.mob.velocity = 1;
	map->editor.settings.flat_z = 0;
	map->editor.settings.inclined.z1 = 0;
	map->editor.settings.inclined.z2 = 100;
	map->editor.settings.texture = ft_strdup("Brique.png");
	map->editor.settings.object.z = 0;
	map->editor.settings.object.width = 30;
	map->editor.place_step = 0;
	map->editor.selected_poly = NULL;
	map->editor.selected_mob = NULL;
	map->editor.selected_obj = NULL;
	map->editor.selected_player = NULL;
	map->editor.placing_poly = NULL;
	map->editor.flags = ED_NONE;
	map->editor.calc = ED_CALC_NORMAL;
	map->editor.arg_menu_tab[0] = (t_arg_menu){(int*)&map->editor.flags,
											ED_SELECTION};
	map->editor.arg_menu_tab[1] = (t_arg_menu){(int*)&map->editor.flags,
											ED_WALL};
	map->editor.arg_menu_tab[2] = (t_arg_menu){(int*)&map->editor.flags,
											ED_FLAT};
	map->editor.arg_menu_tab[3] = (t_arg_menu){(int*)&map->editor.flags,
											ED_INCLINED};
	map->editor.arg_menu_tab[4] = (t_arg_menu){(int*)&map->editor.flags,
											ED_PLAYER};
	map->editor.arg_menu_tab[5] = (t_arg_menu){(int*)&map->editor.calc,
											ED_CALC_NORMAL};
	map->editor.arg_menu_tab[6] = (t_arg_menu){(int*)&map->editor.calc,
											ED_CALC_Z};
	map->editor.arg_menu_tab[7] = (t_arg_menu){(int*)&map->editor.flags,
											ED_MOB};
	map->editor.arg_menu_tab[8] = (t_arg_menu){(int*)&map->editor.flags,
											ED_HEAL};
	map->editor.arg_menu_tab[9] = (t_arg_menu){(int*)&map->editor.flags,
											ED_SHIELD};
	map->editor.arg_menu_tab[10] = (t_arg_menu){(int*)&map->editor.flags,
											ED_GRAVITY};
	map->editor.arg_menu_tab[11] = (t_arg_menu){(int*)&map->editor.flags,
											ED_BULLET};
	map->editor.arg_menu_tab[12] = (t_arg_menu){(int*)&map->editor.flags,
											ED_BOX};
	map->editor.cursor[CURSOR_DEFAULT] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	map->editor.cursor[CURSOR_SELECTING] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
	map->editor.export.path = ft_strdup("./maps/new_map");
	map->editor.export.map = map;
	if (!init_editor_menu(win, map))
		return (ui_ret_error("editor_loop", "init_editor_menu failed", 0));
	loop = SDL_TRUE;
	// ed_delete_mob_polys(map);
	// ed_delete_object_polys(map);
	while (loop)
	{
		
		editor_menu_disp(win, map);
		if (win->winui->event.type == SDL_QUIT)
			loop = 0;
		// check_map(win, &map);
		// editor_event(win, &map, &loop);
		ed_event(win, map);
	}
	editor_menu_quit(win, map, 500);
	init_main_menu(win);
	// editor_quit(win, &map);
	return (1);
}
