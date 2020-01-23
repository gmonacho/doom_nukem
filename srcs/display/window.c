#include "doom_nukem.h"
#include "ui_error.h"

int 			create_window(t_win *win, const char *title, SDL_Rect rect, Uint32 flags)
{
	if (!(win->ptr = SDL_CreateWindow(title, rect.x, rect.y, rect.w, rect.h, flags)))
		return (ret_error(SDL_GetError()));
	win->w = rect.w;
	win->h = rect.h;
	printf("w = %d| h = %d\n", win->w, win->h);
	win->frames = NULL;
	win->text_entry = NULL;
	if (!(win->rend = SDL_CreateRenderer(win->ptr, 0, SDL_RENDERER_SOFTWARE)) ||\
		!(win->rend_texture = SDL_CreateTexture(win->rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT)))
		return (ret_error(SDL_GetError()));
	SDL_SetRenderDrawBlendMode(win->rend, SDL_BLENDMODE_BLEND);
	if (!(win->winui = (t_winui*)ft_memalloc(sizeof(t_winui))))
		return (ui_ret_error("create_window", "win->winui allocation failed", 0));
	win->winui->ptr = win->ptr;
	win->winui->rend = win->rend;
	SDL_PollEvent(&win->winui->event);
	ui_event_update_mouse(&win->winui->mouse);
	win->winui->ui.frames = NULL;
	win->winui->ui.on_mouse_button = NULL;
	win->winui->ui.clicked_button = NULL;
	win->winui->ui.button_font = NULL;
	win->winui->ui.button_text_color = (SDL_Color){255, 255, 255, 255};
	win->winui->ui.button_text_ratio = 0.7;
	win->winui->ui.cursor_position = 0;
	win->winui->ui.cursor_color = (SDL_Color){255, 255, 255, 255};
	win->winui->ui.textures = NULL;
	return (1);
}

void		add_frame_to_window(t_win *win, t_frame *new_frame)
{
	new_frame->rect.x = win->w * new_frame->ratio.x;
	new_frame->rect.y = win->h * new_frame->ratio.y;
	new_frame->rect.w = win->w * new_frame->ratio.w;
	new_frame->rect.h = win->h * new_frame->ratio.h;
	new_frame->next = win->frames;
	win->frames = new_frame;
}

SDL_bool		is_in_screen(t_win *win, t_dot p)
{
	return ((p.x >= 0 && p.x < win->w && p.y >= 0 && p.y < win->h));
}

