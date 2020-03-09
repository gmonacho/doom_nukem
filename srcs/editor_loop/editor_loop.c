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
			else if (*(arg_menu->loop) & ED_DOOR)
				*(arg_menu->loop) ^= ED_DOOR;
			else if (*(arg_menu->loop) & ED_PLAYER)
				*(arg_menu->loop) ^= ED_PLAYER;
			else if (*(arg_menu->loop) & ED_MOB)
				*(arg_menu->loop) ^= ED_MOB;
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
								"b_door",
								&set_editor_flags,
								&map->editor.arg_menu_tab[13]);
	ui_set_simple_button_function(win->winui,
								"b_export",
								&ed_export,
								&map->editor.export);
	ui_set_simple_button_function(win->winui,
								"b_package",
								&ed_package,
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

static void		init_map_editor_settings(t_map *map)
{
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
}

static void		init_map_editor_arg_tab(t_map *map)
{
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
	map->editor.arg_menu_tab[13] = (t_arg_menu){(int*)&map->editor.flags,
											ED_DOOR};
}

static void		init_map_export(t_win *win, t_map *map)
{
	t_text_entry_button	*text_entry;

	if ((text_entry = ui_get_text_entry_button(win->winui, "b_export_path")))
	{
		printf("path = %s\n", map->editor.export.path);
		ft_strcpy(text_entry->text, map->editor.export.path);
	}
	map->editor.export.fd = 0;
	map->editor.export.map = map;
}

static void		init_map_editor(t_win *win, t_map *map)
{
	map->editor.pos = (t_dot){0, 0};
	map->editor.size = (t_dot){0, 0};
	map->editor.unit = 1;
	map->editor.z_min = 0;
	map->editor.z_max = 100;
	map->editor.place_step = 0;
	map->editor.selected_poly = NULL;
	map->editor.selected_mob = NULL;
	map->editor.selected_obj = NULL;
	map->editor.selected_player = NULL;
	map->editor.placing_poly = NULL;
	map->editor.flags = ED_NONE;
	map->editor.calc = ED_CALC_NORMAL;
	init_map_export(win, map);
	init_map_editor_settings(map);
	init_map_editor_arg_tab(map);
	map->editor.cursor[CURSOR_DEFAULT] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	map->editor.cursor[CURSOR_SELECTING] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
}

int				editor_loop(t_win *win, t_map *map)
{
	SDL_bool			loop;

	if (!init_editor_menu(win, map))
		return (ui_ret_error("editor_loop", "init_editor_menu failed", 0));
	init_map_editor(win, map);
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
