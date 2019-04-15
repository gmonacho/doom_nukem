#ifndef DISPLAY_H
# define DISPLAY_H

#include "SDL.h"
#include "libft.h"
#include "ret_error.h"
#include "struct_2d.h"

/*
** ================================== Window Management ==================================
*/

typedef struct		s_win
{
	SDL_Window*		ptr;
	int				w;
	int			h;
	SDL_Renderer*	rend;
}					t_win;

int			create_window(t_win *win, const char *title, SDL_Rect rect, Uint32 flags);
int			game_loop(t_win *win);

/*
** ================================== Drawing Fonctions ==================================
*/

void	draw_line(t_win *win, t_dot p1, t_dot p2);

#endif