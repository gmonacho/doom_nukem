#include "doom_nukem.h"

static int		ui_texture_init(t_win *win)
{
	SDL_Surface		*surface;
	SDL_Texture		*t;
	char			*str;
	int				i;

	if (!(win->font = TTF_OpenFont("font/font.ttf", 18)))
		return (0);
	if (!(win->sectors_texture = (SDL_Texture**)ft_memalloc(sizeof(SDL_Texture*) * (MAX_SECTORS + 1))))
		return (0);
	if (!(win->sectors_texture_selected = (SDL_Texture**)ft_memalloc(sizeof(SDL_Texture*) * (MAX_SECTORS + 1))))
		return (0);
	if (!(win->ed_texture.frame_texture = load_texture(win->rend, "textures/frame.png")))
		return (ret_error(SDL_GetError()));
	if (!(win->ed_texture.button = load_texture(win->rend, "textures/button.png")))
		return (ret_error(SDL_GetError()));
	if (!(win->ed_texture.clicked_button = load_texture(win->rend, "textures/clicked_button.png")))
		return (ret_error(SDL_GetError()));
	if (!(win->ed_texture.on_mouse_button = load_texture(win->rend, "textures/on_mouse_button.png")))
		return (ret_error(SDL_GetError()));
	i = 0;
	while (i <= MAX_SECTORS)
	{
		if (i < MAX_SECTORS)
			str = ft_itoa(i);
		else
		{
			str[0] = '+';
			str[1] = '\0';
		}
		if (!(surface = TTF_RenderText_Solid(win->font, str, (SDL_Color){200, 200, 200, 255})))
			return (0);
		if (!(t = SDL_CreateTextureFromSurface(win->rend, surface)))
			return (0);
		SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
		if (!(win->sectors_texture[i] = SDL_CreateTexture(win->rend, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 30, 30)))
			return (0);
		if (!(win->sectors_texture_selected[i] = SDL_CreateTexture(win->rend, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 30, 30)))
			return (0);
		SDL_SetTextureBlendMode(win->sectors_texture[i], SDL_BLENDMODE_BLEND);
		SDL_SetTextureBlendMode(win->sectors_texture_selected[i], SDL_BLENDMODE_BLEND);

		SDL_SetRenderTarget(win->rend, win->sectors_texture[i]);
		SDL_SetRenderDrawColor(win->rend, 0, 0, 0, 0);
		SDL_RenderClear(win->rend);
		SDL_RenderCopy(win->rend, t, NULL, &(SDL_Rect){10, 10, 12, 12});

		SDL_SetRenderTarget(win->rend, win->sectors_texture_selected[i]);
		SDL_SetRenderDrawColor(win->rend, 70, 70, 70, 100);
		SDL_RenderClear(win->rend);
		SDL_RenderCopy(win->rend, t, NULL, &(SDL_Rect){10, 10, 12, 12});

		SDL_DestroyTexture(t);
		SDL_FreeSurface(surface);
		i++;
	}
	if (!(win->text_entry_texture = create_bg_text_input(win, (SDL_Rect){100, -1, 300, 100},
																(SDL_Color){80, 80, 80, 255},
																(SDL_Color){40, 40, 40, 255})))
		return (ret_error("bg_text_input creation failed in text init"));
	SDL_SetRenderTarget(win->rend, NULL);
	return (1);
}

static int		ui_init(t_win *win)
{
	SDL_Texture	*text;
	SDL_Texture	*t;

	//	sector frame
	add_frame_to_window(win, new_frame((t_frect){0.05, 0.02, 0.9, 0.05}, win->ed_texture.frame_texture, FRAME_SECTORS, NULL));
	add_button_to_frame(&win->frames, new_button((t_frect){0, 0, 1.0 / MAX_SECTORS, 1}, win->sectors_texture[MAX_SECTORS], BUTTON_NONE));
	//	info sector frame
	add_frame_to_window(win, new_frame((t_frect){0.02, 0.1, 0.15, 0.4}, win->ed_texture.frame_texture, FRAME_INFO | FRAME_HIDE, NULL));
	//		text_input
	//			name
	if (!(text = generate_text(win->rend, win->font, "name",  (SDL_Color){200, 200, 200, 255})))
		return (ret_error("text generation failed in ui_init"));
	if (!(t = blit_text(win->rend, win->text_entry_texture, text, &(SDL_Rect){10, 24, 80, 75})))
		return (ret_error("blit_text failed in ui_init"));
	add_button_to_frame(&win->frames, new_button((t_frect){0.1, 0.1, 0.8, 0.05}, t, BUTTON_TEXT_ENTRY | BUTTON_SECTOR_NAME));
	win->frames->buttons->data = new_text_entry("name", 8, NULL, TEXT_ENTRY_ALPHANUM);
	//		color_picker
	// add_button_to_frame(&win->frames, new_button((t_frect){0.1, 0.8, 0.8, 0.05}, NULL, BUTTON_COLOR_PICKER));
	//		export_button
	add_button_to_frame(&win->frames, new_button((t_frect){0.65, 0.875, 0.3, 0.04}, win->ed_texture.button, BUTTON_EXPORT | BUTTON_SIMPLE));
	if (!(win->frames->buttons->data = new_simple_button("export", SDL_FALSE)))
		return (ret_error("new_simple_button failed export button data"));
	update_button(win, win->frames->buttons, BUTTON_STATE_NONE);
	//	info linedef frame
	add_frame_to_window(win, new_frame((t_frect){0.02, 0.55, 0.15, 0.4}, win->ed_texture.frame_texture, FRAME_L_INFO, NULL));
	//		text_input
	//			id
	if (!(text = generate_text(win->rend, win->font, "id",  (SDL_Color){200, 200, 200, 255})))
		return (ret_error("text generation failed in ui_init"));
	if (!(t = blit_text(win->rend, win->text_entry_texture, text, &(SDL_Rect){10, 24, 80, 75})))
		return (ret_error("blit_text failed in ui_init"));
	add_button_to_frame(&win->frames, new_button((t_frect){0.1, 0.05, 0.4, 0.05}, t, BUTTON_TEXT_ENTRY | BUTTON_ID));
	if (!(win->frames->buttons->data = new_text_entry("id", 2, NULL, TEXT_ENTRY_DIGITAL | TEXT_ENTRY_TMP)))
		return (ret_error("new_text_entry failed id button data"));
	//		button_glinedef
	add_button_to_frame(&win->frames, new_button((t_frect){0.1, 0.2, 0.8, 0.05}, NULL, BUTTON_L_TYPE | BUTTON_SIMPLE));
	if (!(win->frames->buttons->data = new_simple_button("line type", SDL_FALSE)))
		return (ret_error("new_simple_button failed glinedef button data"));
	update_button(win, win->frames->buttons, BUTTON_STATE_NONE);
	//		frame_glinedef
	add_frame_to_window(win, new_frame((t_frect){0.175, 0.63, 0.15, 0.2}, win->ed_texture.frame_texture, FRAME_HIDE | FRAME_L_TYPE, NULL));
	//			linedef_type_buttons
	add_button_to_frame(&win->frames, new_button((t_frect){0, 0, 1, 0.33}, NULL, BUTTON_NONE | BUTTON_SIMPLE));
	if (!(win->frames->buttons->data = new_simple_button("WALL", SDL_FALSE)))
		return (ret_error("new_simple_button failed glinedef button data"));
	update_button(win, win->frames->buttons, BUTTON_STATE_NONE);
	win->frames->buttons->gflags = WALL;
	add_button_to_frame(&win->frames, new_button((t_frect){0, 0.33, 1, 0.33}, NULL, BUTTON_NONE | BUTTON_SIMPLE));
	if (!(win->frames->buttons->data = new_simple_button("PORTAL", SDL_FALSE)))
		return (ret_error("new_simple_button failed glinedef button data"));
	update_button(win, win->frames->buttons, BUTTON_STATE_NONE);
	win->frames->buttons->gflags = PORTAL;
	return (1);
}

static int		editor_init(t_win *win, t_map_editor *map)
{
	map->name = "Unnamed";
	map->x = 0;
	map->y = 0;
	map->w = 2000;
	map->h = 1200;
	map->unit = 1.0;
	map->sectors = NULL;
	map->selected_sector = NULL;
	map->rect_util = (SDL_Rect){};
	map->flags = 0;
	if (!ui_texture_init(win))
		return (0);
	if (!ui_init(win))
		return (ret_error("ui_init failed in editor_init"));
	return (1);
}

int				editor_loop(t_win *win)
{
	SDL_bool			loop;
	t_map_editor		map;

	// if (!(parser_png("png_test_800_600.png")))
	// 	return (0);
	if (!editor_init(win, &map))
		return (ret_error("editor_init failed in editor loop"));
	loop = SDL_TRUE;
	while (loop)
	{
		clear_rend(win->rend, 30, 30, 30);
		editor_display(win, &map);
		editor_event(win, &map, &loop);
		SDL_RenderPresent(win->rend);
	}
	return (1);
}