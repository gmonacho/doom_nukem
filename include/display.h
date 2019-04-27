#ifndef DISPLAY_H
# define DISPLAY_H

# include "SDL.h"
# include "libft.h"
# include "ret_error.h"
# include "event.h"
# include "struct_2d.h"

/*
** ================================== Window Management ==================================
*/

typedef struct		s_win
{
	SDL_Window*		ptr;
	int				w;
	int				h;
	SDL_Renderer*	rend;
	t_mouse			*mouse;
}					t_win;

int			create_window(t_win *win, const char *title, SDL_Rect rect, Uint32 flags);
SDL_bool	is_in_screen(t_win *win, t_dot p);

/*
** ================================== Renderer Management ==================================
*/

void		clear_rend(SDL_Renderer *rend, Uint8 r, Uint8 g, Uint8 b);

/*
** ================================== Drawing Fonctions ==================================
*/

void		draw_line(t_win *win, t_dot p1, t_dot p2);
void		draw_rect(t_win *win, SDL_Rect rect);
void		draw_circle(t_win *win, t_circle circle);

#endif