#ifndef BORDEL_PROTO_H
# define BORDEL_PROTO_H

# include "SDL.h"

/*
**	====================== Mouse Managment ======================
*/

t_mouse		*mouse_refresh();
t_dot		mouse_drag(int x, int y, SDL_bool end);

/*
**	====================== Event Managment ======================
*/

int		keyboard_state(t_map *map, t_player *player);
int		key_pressed(Uint32 sdl_keycode);

/*
** ==================== Error Management ==========================
*/

int			ret_error(const char *error_msg);
void		*ret_null_error(const char *error_msg);
int			ret_perror(const char *error_msg);
void		*ret_null_perror(const char *error_msg);

/*
**	====================== Time Managment ======================
*/

void	draw_fps();

/*
** ======================= Window Management =====================
*/

int			create_window(t_win *win, const char *title, SDL_Rect rect, Uint32 flags);
SDL_bool	is_in_screen(t_win *win, t_dot p);

/*
** ========================= Renderer Management =================
*/

void		clear_rend(SDL_Renderer *rend, Uint8 r, Uint8 g, Uint8 b);

/*
** ======================= Drawing Fonctions =====================
*/

void		draw(t_win *win, t_map *map, t_player *player);
void		draw_line(t_win *win, t_fdot p1, t_fdot p2);

/*
**	=========================== Map ==============================
*/

void		map_add_line(t_map *map, int n_sector, t_linedef *line);
int			map_get_nb_data(void **data);
t_linedef	*new_linedef(t_line line, SDL_Texture *texture, Uint32 flags);
void		map_zoom(t_win *win, t_map *map);

/*
** ======================== Game Management =====================
*/

int			game_loop(t_win *win, t_map *map);
int			move(t_map *map, t_player *player);

/*
** ========================== Math functions ======================
*/

double		dist(t_fdot d1, t_fdot d2);
double		mag(t_vector vector);

#endif
