#include "doom_nukem.h"

static int		text_init(t_win *win)
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
	add_frame_to_window(win, new_frame((t_frect){0.05, 0.02, 0.9, 0.05}, NULL, FRAME_SECTORS, NULL));
	add_button_to_frame(&win->frames, new_button((t_frect){0, 0, 1.0 / MAX_SECTORS, 1}, win->sectors_texture[MAX_SECTORS], BUTTON_NONE));
	//	info sector frame
	add_frame_to_window(win, new_frame((t_frect){0.02, 0.1, 0.15, 0.4}, NULL, FRAME_INFO | FRAME_HIDE, NULL));
	//		text_input
	//			name
	if (!(text = generate_text(win->rend, win->font, "name",  (SDL_Color){200, 200, 200, 255})))
		return (ret_error("text generation failed in ui_init"));
	if (!(t = blit_text(win->rend, win->text_entry_texture, text, &(SDL_Rect){10, 24, 80, 75})))
		return (ret_error("blit_text failed in ui_init"));
	add_button_to_frame(&win->frames, new_button((t_frect){0.1, 0.1, 0.8, 0.05}, t, BUTTON_TEXT_ENTRY));
	//		color_picker
	add_button_to_frame(&win->frames, new_button((t_frect){0.1, 0.8, 0.8, 0.05}, NULL, BUTTON_COLOR_PICKER));
	//		export_button
	add_button_to_frame(&win->frames, new_button((t_frect){0.7, 0.925, 0.2, 0.03}, NULL, BUTTON_EXPORT));
	//	info linedef frame
	add_frame_to_window(win, new_frame((t_frect){0.02, 0.55, 0.15, 0.4}, NULL, FRAME_L_INFO, NULL));
	return (1);
}

static int		editor_init(t_win *win, t_map_editor *map)
{
	map->x = 0;
	map->y = 0;
	map->w = 2000;
	map->h = 1200;
	map->unit = 1.0;
	map->sectors = NULL;
	map->selected_sector = NULL;
	map->rect_util = (SDL_Rect){};
	map->flags = 0;
	if (!text_init(win))
		return (0);
	ui_init(win);
	return (1);
}

int				editor_loop(t_win *win)
{
	SDL_bool			loop;
	t_map_editor		map;

	// if (!(parser_png("png_test_800_600.png")))
	// 	return (0);
	editor_init(win, &map);
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