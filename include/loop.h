#ifndef LOOP_H
# define LOOP_H

# include "display.h"
# include "data.h"

/*
** ================================== Loops ==================================
*/

int			game_loop(t_win *win);

int			editor_loop(t_win *win);
int			editor_event(t_win *win, t_map *map, SDL_bool *loop);

#endif