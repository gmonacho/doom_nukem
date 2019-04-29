#ifndef DISPLAY_H
# define DISPLAY_H

# include "SDL.h"
# include "libft.h"

# include "event.h"
# include "data.h"

/*
** ================================== Window Management ==================================
*/

typedef struct		s_win
{
	SDL_Window		*ptr;
	int				width;
	int				height;
	SDL_Renderer	*rend;
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

void		draw(t_win *win, t_map *map, t_player *player);
void		draw_line(t_win *win, t_fdot p1, t_fdot p2);

#endif