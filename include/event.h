#ifndef EVENT_H
# define EVENT_H

# include "SDL.h"

# include "struct_2d.h"
# include "loop.h"
# include "display.h"
# include "physics.h"
# include "data.h"

enum mouse_button {MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT};

typedef struct	s_button
{
	SDL_bool	pressed;
	SDL_bool	pressing;
	SDL_bool	releasing;
}				t_button;

/*
**	====================== Mouse Managment ======================
*/

typedef struct	s_mouse
{
	int			x;
	int			y;
	Uint32		flags;
	t_button	button[3];
}				t_mouse;

t_mouse		*mouse_refresh();
t_dot		mouse_drag(int x, int y, SDL_bool end);

/*
**	====================== Event Managment ======================
*/

int		keyboard_state(t_map *map, t_player *player);
int		key_pressed(Uint32 sdl_keycode);

/*
**	====================== Time Managment ======================
*/

void	draw_fps();

#endif