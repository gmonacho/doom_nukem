#include "doom_nukem.h"

static void	main_menu_quit(t_win *win)
{
	free_frames(&win->frames);
}


static void	main_menu_display(t_win *win)
{
	t_frame *f;
	t_button *b;

	SDL_SetRenderDrawColor(win->rend, 200, 200, 200, 255);
	f = win->frames;
	while (f)
	{
		if (!(f->flags & FRAME_HIDE))
		{
			if (f->texture)
				printf("texture\n");
			else
				draw_ratio_rect(win, &(SDL_Rect){0, 0, win->w, win->h}, &f->ratio);
			b = f->buttons;
			while (b)
			{
				if (b->texture)
					SDL_RenderCopy(win->rend, b->texture, NULL, &b->rect);
				else
					draw_ratio_rect(win, &f->rect, &b->ratio);
				b = b->next;
			} 
		}
		f = f->next;
	}
}


static int	main_menu_click(t_win *win)
{
	if (win->selected_button)
	{
		if (win->selected_button->flags & BUTTON_GAMELOOP)
			return (2);
		else if (win->selected_button->flags & BUTTON_EDITORLOOP)
			return (3);
		else if (win->selected_button->flags & BUTTON_MENU_QUIT)
			return (4);
		else if(win->selected_button->flags & BUTTON_MENU_CREDIT)
			return (5);
		else if(win->selected_button->flags & BUTTON_CREDIT_RETURN)
			return (6);
	}
	return (1);
}

static int	main_menu_event(t_win *win, int *loop)
{	
	SDL_Event	event;

	if (win)
	{
		while (SDL_PollEvent(&event))
		{
			mouse_refresh();
			if (event.type == SDL_QUIT || event.key.keysym.scancode  == SDL_SCANCODE_ESCAPE)
				*loop = SDL_FALSE;
			else if (event.type == SDL_MOUSEMOTION)
				update_selected_ui(win);
			else if(event.type == SDL_WINDOWEVENT)
			{
				if(event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(win->ptr, &win->w, &win->h);
					update_ui_rect(win);
					printf("winw = %d | winh = %d\n", win->w, win->h);
				}
			}
			if (win->mouse->button[MOUSE_LEFT].releasing)
				return (main_menu_click(win));
		}
	}
	return (1);
}

static int	main_menu_init(t_win *win)
{
	if (!(win->main_menu = (t_main_menu*)ft_memalloc(sizeof(t_main_menu))))
	{	
		ft_putendl("Error malloc main_menu.c l.94\n");
		exit(0);
	}
	win->main_menu->police = TTF_OpenFont("TTF/DooM.ttf", 65);
	win->main_menu->text[0] = generate_text(win->rend, win->main_menu->police, "Play", (SDL_Color){255, 0, 0, 50});
	win->main_menu->text[1] = generate_text(win->rend, win->main_menu->police, "Map Editor", (SDL_Color){255, 0, 0, 50});
	win->main_menu->text[2] = generate_text(win->rend, win->main_menu->police, "Credits", (SDL_Color){255, 0, 0, 50});
	win->main_menu->text[3] = generate_text(win->rend, win->main_menu->police, "Quit", (SDL_Color){255, 0, 0, 50});
	win->main_menu->textb[0] = generate_text(win->rend, win->main_menu->police, "Play", (SDL_Color){0, 0, 0, 50});
	win->main_menu->textb[1] = generate_text(win->rend, win->main_menu->police, "Map Editor", (SDL_Color){0, 0, 0, 50});
	win->main_menu->textb[2] = generate_text(win->rend, win->main_menu->police, "Credits", (SDL_Color){0, 0, 0, 50});
	win->main_menu->textb[3] = generate_text(win->rend, win->main_menu->police, "Quit", (SDL_Color){0, 0, 0, 50});
	//	menu frame
	add_frame_to_window(win, new_frame((t_frect){-0.01, -0.01, 1.01, 1.01}, NULL, FRAME_NONE, NULL));
	//	game_loop_button
	add_button_to_frame(&win->frames, new_button((t_frect){0.35, 0.5, 0.3, 0.08}, win->main_menu->text[0], BUTTON_GAMELOOP));
	//	editor_loop_button
	add_button_to_frame(&win->frames, new_button((t_frect){0.35, 0.6, 0.3, 0.08}, win->main_menu->text[1], BUTTON_EDITORLOOP));
	//	credit menu button
	add_button_to_frame(&win->frames, new_button((t_frect){0.35, 0.7, 0.3, 0.08}, win->main_menu->text[2], BUTTON_MENU_CREDIT));
	// quit menu button
	add_button_to_frame(&win->frames, new_button((t_frect){0.35, 0.8, 0.3, 0.08}, win->main_menu->text[3], BUTTON_MENU_QUIT));
	return (1);
}

void	print_back_text(t_win *win)
{
	int i;
	float pos;

	i = 0;
	pos = 0.5;
	while(i < 4)
	{
		SDL_RenderCopy(win->rend, win->main_menu->textb[i], NULL, &(SDL_Rect){(0.35 * win->w), (pos * win->h), (0.3 * win->w), (0.08 * win->h)});
		pos += 0.1;
		i++;
	}
}

int			main_menu(t_win *win)
{
	int			loop;
	int			next_loop;
	SDL_Texture	*background;

	background = load_texture(win->rend, "textures/imageMenu.png");
	main_menu_init(win);
	loop = SDL_TRUE;
	SDL_SetRelativeMouseMode(SDL_FALSE);
	while (loop)
	{
		clear_rend(win->rend, 71, 27, 27);
		SDL_RenderCopy(win->rend, background, NULL, &(SDL_Rect){(0), (0.15 * win->h), (win->w), (0.7 * win->h)});
		print_back_text(win);
		main_menu_display(win);
		next_loop = main_menu_event(win, &loop);
		if (next_loop > 1)
			loop = SDL_FALSE;
		SDL_RenderPresent(win->rend);
	}
	main_menu_quit(win);
	return (next_loop);                      
}