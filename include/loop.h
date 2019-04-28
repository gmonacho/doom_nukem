#ifndef LOOP_H
# define LOOP_H

# include "struct_2d.h"
# include "event.h"
# include "display.h"
# include "physics.h"
# include "data.h"

/*
** ================================== Loops ==================================
*/

int			game_loop(t_win *win, t_map *map);

int			editor_loop(t_win *win);
int			editor_event(t_win *win, t_map *map, SDL_bool *loop);

#endif
