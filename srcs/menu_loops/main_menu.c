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
			if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				*loop = SDL_FALSE;
				printf("QUIT\n");
			}
			else if (event.type == SDL_MOUSEMOTION)
				update_selected_ui(win);
			else if(event.type == SDL_WINDOWEVENT)
			{
				if(event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(win->ptr, &win->w, &win->h);
					update_ui_rect(win);
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
	SDL_Texture *text[4];
	TTF_Font	*police;

	police = TTF_OpenFont("TTF/DooM.ttf", 65);
	text[0] = generate_text(win->rend, police, "Play", (SDL_Color){255, 0, 0, 50});
	text[1] = generate_text(win->rend, police, "Map Editor", (SDL_Color){255, 0, 0, 50});
	text[2] = generate_text(win->rend, police, "Credits", (SDL_Color){255, 0, 0, 50});
	text[3] = generate_text(win->rend, police, "Quit", (SDL_Color){255, 0, 0, 50});
	//	menu frame
<<<<<<< HEAD
	add_frame_to_window(win, new_frame((t_frect){-0.01, -0.01, 1.01, 1.01}, NULL, FRAME_NONE, NULL));
	//	game_loop_button
	add_button_to_frame(&win->frames, new_button((t_frect){0.35, 0.5, 0.3, 0.08}, text[0], BUTTON_GAMELOOP));
	//	editor_loop_button
	add_button_to_frame(&win->frames, new_button((t_frect){0.35, 0.6, 0.3, 0.08}, text[1], BUTTON_EDITORLOOP));
	//	credit menu button
	add_button_to_frame(&win->frames, new_button((t_frect){0.35, 0.7, 0.3, 0.08}, text[2], BUTTON_MENU_CREDIT));
	// quit menu button
	add_button_to_frame(&win->frames, new_button((t_frect){0.35, 0.8, 0.3, 0.08}, text[3], BUTTON_MENU_QUIT));
=======
	add_frame_to_window(win, new_frame((t_frect){0, 0, 1, 1}, NULL, FRAME_NONE, NULL));
	//		game_loop_button
	add_button_to_frame(&win->frames, new_button((t_frect){0.3, 0.4, 0.15, 0.1}, NULL, BUTTON_GAMELOOP));
	//		editor_loop_button
	add_button_to_frame(&win->frames, new_button((t_frect){0.55, 0.4, 0.15, 0.1}, NULL, BUTTON_EDITORLOOP));
>>>>>>> 42dc125c1d965efbf5d7bfb235076d9c8b6dce6f
	return (1);
}

int			main_menu(t_win *win)
{
	int			loop;
	int			next_loop;
	SDL_Texture	*background;

	background = load_texture(win->rend, "textures/imageMenu.png");
	main_menu_init(win);
	loop = SDL_TRUE;
	while (loop)
	{
		clear_rend(win->rend, 30, 30, 30);
		SDL_RenderCopy(win->rend, background, NULL, &(SDL_Rect){(0), (0.25 * win->h), (win->w), (0.5 * win->h)});
		main_menu_display(win);
		next_loop = main_menu_event(win, &loop);
		if (next_loop > 1)
			loop = SDL_FALSE;
		SDL_RenderPresent(win->rend);
	}
	main_menu_quit(win);
	return (next_loop);
}