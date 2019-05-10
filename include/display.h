#ifndef DISPLAY_H
# define DISPLAY_H

# include "SDL.h"
# include "libft.h"
# include "ret_error.h"
# include "event.h"
# include "struct_2d.h"
# include "SDL_ttf.h"

/*
** ================================== Interface Management ==================================
*/

enum	e_button
{
	BUTTON_NONE = 0x0000,
	BUTTON_COLOR_PICKER = 0x0001
};

typedef struct		s_button
{
	t_frect			ratio;
	SDL_Rect		rect;
	SDL_Texture		*texture;
	Uint32			flags;
	struct s_button	*next;
}					t_button;

t_button	*new_button(const t_frect ratio, SDL_Texture *texture, Uint32 button_flags);
void		add_button(t_button **buttons, t_button *new_button);

enum	e_frame
{
	FRAME_NONE = 0x0000,
	FRAME_SECTORS = 0x0001,
	FRAME_INFO = 0x0010
};

typedef struct		s_frame
{
	t_frect			ratio;
	SDL_Rect		rect;
	SDL_Texture		*texture;

	t_button		*buttons;
	int				nb_buttons;

	Uint32			flags;

	struct s_frame	*next;
}					t_frame;

t_frame		*new_frame(const t_frect ratio, SDL_Texture *texture, Uint32 frame_flags, t_button *buttons);
void		add_frame(t_frame **frames, t_frame *new_frame);
void		add_button_to_frame(t_frame **frame, t_button *button);

/*
** ================================== Window Management ==================================
*/

typedef struct		s_win
{
	SDL_Window*		ptr;
	SDL_Renderer*	rend;

	int				w;
	int				h;

	t_mouse			*mouse;

	SDL_Texture		**sectors_texture;
	SDL_Texture		**sectors_texture_selected;

	float			picker_position;

	t_frame			*frames;
	t_frame			*selected_frame;

	t_button		*selected_button;

	TTF_Font		*font;
}					t_win;

int			create_window(t_win *win, const char *title, SDL_Rect rect, Uint32 flags);
SDL_bool	is_in_screen(t_win *win, t_dot p);
void		add_frame_to_window(t_win *win, t_frame *new_frame);

/*
** ================================== Renderer Management ==================================
*/

void		clear_rend(SDL_Renderer *rend, Uint8 r, Uint8 g, Uint8 b);

/*
** ================================== Drawing Fonctions ==================================
*/

void		draw_line(t_win *win, t_dot p1, t_dot p2);
void		draw_rect(t_win *win, SDL_Rect rect);
void		draw_ratio_rect(t_win *win, const SDL_Rect *rect, const t_frect *ratio);
void		draw_circle(t_win *win, t_circle circle);
void		draw_color_picker(t_win *win, SDL_Rect rect);

#endif