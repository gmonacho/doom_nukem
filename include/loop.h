#ifndef LOOP_H
# define LOOP_H

# include "display.h"
# include "data.h"
# include "SDL.h"

/*
** ================================== Loops ==================================
*/

enum	e_sc	// shortcut
{
	SC_NONE = 0,
	SC_DRAW_FREE = SDL_SCANCODE_SPACE
};

int			game_loop(t_win *win);
int			editor_loop(t_win *win);
void		resolve_ui_left_release(t_win *win, t_map *map);
int			editor_event(t_win *win, t_map *map, SDL_bool *loop);

#endif