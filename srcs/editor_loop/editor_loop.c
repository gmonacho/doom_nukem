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
			printf("la\n");
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
			else if (*(arg_menu->loop) & ED_DOOR)
				*(arg_menu->loop) ^= ED_DOOR;
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

int				editor_loop(t_win *win, t_map *map)
{
	SDL_bool			loop;

	if (!init_editor_menu(win, map))
		return (ui_ret_error("editor_loop", "init_editor_menu failed", 0));
	ed_init_map_editor(win, map);
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
