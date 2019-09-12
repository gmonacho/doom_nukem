#include "doom_nukem.h"

static int		ui_texture_init(t_win *win)
{
	SDL_Surface		*surface;
	SDL_Texture		*t;
	char			*str;
	int				i;

	if (!(win->font.digital = TTF_OpenFont("font/font.ttf", 18)))
		return (0);
	if (!(win->font.ui = TTF_OpenFont("font/BebasNeue-Regular.ttf", 25)))
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
	while (i < 10)
	{
		if (!(surface = TTF_RenderText_Solid(win->font.digital, ft_itoa(i), (SDL_Color){200, 200, 200, 255})))
			return (0);
		if (!(win->ed_texture.digit_tab[i] = SDL_CreateTextureFromSurface(win->rend, surface)))
			return (0);
		i++;
	}
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
		if (!(surface = TTF_RenderText_Solid(win->font.digital, str, (SDL_Color){200, 200, 200, 255})))
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

static int		set_name_button_variable(t_win *win, const char *name, void *variable)
{
	t_frame			*f;
	t_button		*b;
	t_text_entry	*data;

	f = win->frames;
	while (f)
	{
		b = f->buttons;
		while (b)
		{
			if (b->flags & BUTTON_TEXT_ENTRY)
			{
				data = (t_text_entry*)b->data;
				if (ft_strcmp(data->name, name) == 0)
					data->variable = variable;
			}
			b = b->next;
		}
		f = f->next;
	}
	return (1);
}

static int		set_flags_button_variable(t_win *win, const Uint32 flags, void *variable)
{
	t_frame			*f;
	t_button		*b;
	t_text_entry	*data;

	f = win->frames;
	while (f)
	{
		b = f->buttons;
		while (b)
		{
			if (b->flags & BUTTON_TEXT_ENTRY)
			{
				data = (t_text_entry*)b->data;
				if (b->flags & flags)
					data->variable = variable;
			}
			b = b->next;
		}
		f = f->next;
	}
	return (1);
}

static int		ui_update_text_entry_texture(t_win *win)
{
	t_frame			*f;
	t_button		*b;
	t_text_entry	*data;

	f = win->frames;
	while (f)
	{
		b = f->buttons;
		while (b)
		{
			if (b->flags & BUTTON_TEXT_ENTRY)
			{
				data = (t_text_entry*)b->data;
				if (data->variable)
				{
					if (data->flags & TEXT_ENTRY_ALPHANUM)
					{
						if (!update_text_entry_texture(win, b, (const char*)data->variable))
							return (ret_error("ui_update_text_entry_texture : update_text_entry_texture failed"));
					}
					else if (data->flags & TEXT_ENTRY_DIGITAL)
					{
						if (!update_text_entry_texture(win, b, ft_itoa(*(int*)data->variable)))
							return (ret_error("ui_update_text_entry_texture : update_text_entry_texture failed"));
					}
				}
				else
					if (!update_text_entry_texture(win, b, NULL))
							return (ret_error("ui_update_text_entry_texture : update_text_entry_texture failed"));
				
			}
			b = b->next;
		}
		f = f->next;
	}
	return (1);
}

static int		ui_init_variable(t_win *win, t_map_editor *map)
{
	set_name_button_variable(win, "x", &map->player.dpos.x);
	set_name_button_variable(win, "y", &map->player.dpos.y);
	set_name_button_variable(win, "width", &map->player.width);
	set_name_button_variable(win, "height", &map->player.height);
	set_name_button_variable(win, "sector", &map->player.i_sector);
	set_flags_button_variable(win, BUTTON_MAP_NAME, map->name);
	return (1);
}

static int		ui_init(t_win *win, t_map_editor *map)
{
	int		fd;

	if ((fd = open("srcs/ui/menu_ui", O_RDONLY)) <= 0)
		return (ret_error("ui_init : menu_ui openning failed"));
	win->frames = NULL;
	if (!(load_ui(fd, win)))
		return (ret_error("ui_init : load_ui failed"));
	ui_init_variable(win, map);
	update_ui_rect(win);
	ui_update_text_entry_texture(win);
	ui_update_text_entry_texture(win);
	return (1);
}

static int		editor_init(t_win *win, t_map_editor *map)
{
	if (!(map->name = (char*)ft_memalloc(sizeof(char) * 11)))
		return (ret_error("allocation failed in add_sector"));
	ft_strcpy(map->name, "Unnamed");
	map->x = 0;
	map->y = 0;
	map->w = 2000;
	map->h = 1200;
	map->unit = 1.0;
	map->sectors = NULL;
	map->selected_sector = NULL;
	map->rect_util = (SDL_Rect){};
	map->flags = 0;
	map->player.pos.x = 0;
	map->player.pos.y = 0;
	map->player.dpos.x = 50;
	map->player.dpos.y = 50;
	map->player.width = 50;
	map->player.height = 50;
	map->player.vel = (t_fvector){1, 1};
	map->player.i_sector = 0;
	if (!ui_texture_init(win))
		return (0);
	if (!ui_init(win, map))
		return (ret_error("ui_init failed in editor_init"));
	return (1);
}

int				editor_loop(t_win *win, t_map *game_map)
{
	SDL_bool			loop;
	t_map_editor		map;
	t_sector			*s;
	t_linedef			*l;

	// if (!(parser_png("png_test_800_600.png")))
	// 	return (0);
	if (!editor_init(win, &map))
		return (ret_error("editor_init failed in editor loop"));
	if (game_map)
	{
		map.sectors = game_map->sectors;
		reverse_sectors(&map.sectors);
		s = map.sectors;
		while (s)
		{
			l = s->lines;
			while (l)
			{
				l->gflags = l->flags;
				l->flags = LINEDEF_NONE;
				l = l->next;
			}
			s = s->next;
		}
		map.player = game_map->player;
		map.player.dpos.x = game_map->player.pos.x;
		map.player.dpos.y = game_map->player.pos.y;
	}
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