#ifndef BORDEL_PROTO_H
# define BORDEL_PROTO_H

# include "SDL.h"


/*
** =====================================================================================
** ================================== MAIN ============================================
** =====================================================================================
*/

/*
** =====================================================================================
** ================================== EVENT ============================================
** =====================================================================================
*/

/*
**	---------------------------------- Mouse ----------------------------------
*/

t_mouse		*mouse_refresh();
t_dot		mouse_drag(int x, int y, SDL_bool end);

/*
**	---------------------------------- Event ----------------------------------
*/

int		    keyboard_state(t_player *player);
int		    key_pressed(Uint32 sdl_keycode);

/*
**	---------------------------------- Time ----------------------------------
*/

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

void		draw_fps();

/*
** ============================================================================
** ================================== DISPAY ==================================
** ============================================================================
*/

/*
**	---------------------------------- text ----------------------------------
*/

SDL_Texture		*generate_text(SDL_Renderer *rend, TTF_Font *font, const char *text, SDL_Color fg);
SDL_Texture		*create_bg_text_input(t_win *win, SDL_Rect rect, SDL_Color l_color, SDL_Color r_color);
SDL_Texture		*blit_text(SDL_Renderer *rend, SDL_Texture *bg_texture, SDL_Texture *text, SDL_Rect *text_rect);

/*
**	---------------------------------- button ----------------------------------
*/

t_button	*new_button(const t_frect ratio, SDL_Texture *texture, Uint32 button_flags);
void		add_button(t_button **buttons, t_button *new_button);

/*
**	---------------------------------- frame ----------------------------------
*/

t_frame		*new_frame(const t_frect ratio, SDL_Texture *texture, Uint32 frame_flags, t_button *buttons);
void		add_frame(t_frame **frames, t_frame *new_frame);
void		add_button_to_frame(t_frame **frame, t_button *button);
void		add_frame_flags(t_frame **frame, Uint32 target_flags, Uint32 added_flags);

/*
**	---------------------------------- Window ----------------------------------
*/

int			create_window(t_win *win, const char *title, SDL_Rect rect, Uint32 flags);
SDL_bool	is_in_screen(t_win *win, t_dot p);
void		add_frame_to_window(t_win *win, t_frame *new_frame);
void		remove_frame_flags(t_frame **frame, Uint32 target_flags, Uint32 removed_flags);

/*
**	---------------------------------- Renderer ----------------------------------
*/

void		clear_rend(SDL_Renderer *rend, Uint8 r, Uint8 g, Uint8 b);

/*
**	---------------------------------- Drawing functon ----------------------------------
*/


void		draw_line(t_win *win, t_dot p1, t_dot p2);
void		draw_rect(t_win *win, SDL_Rect rect);
void		fill_rect(t_win *win, SDL_Rect rect);
void		draw_ratio_rect(t_win *win, const SDL_Rect *rect, const t_frect *ratio);
void		draw_circle(t_win *win, t_circle circle);
void	    draw_color_picker(t_win *win, float picker_position, SDL_Rect rect);
void		draw(t_win *win, t_map *map, t_player *player);

/*
** ==========================================================================
** ================================== DATA ==================================
** ==========================================================================
*/

/*
**	---------------------------------- png ----------------------------------
*/

int		    parser_png(char *png_file);

/*
**	---------------------------------- linedef ----------------------------------
*/

t_linedef	*init_linedef(t_linedef *line);
void		add_linedef(t_linedef **lines, t_linedef *new_linedef);
t_linedef	*new_linedef(t_line line, SDL_Texture *texture, Uint32 flags);
t_linedef	*new_void_linedef(void);
int			get_nb_linedef(t_linedef *lines);


/*
**	---------------------------------- sector ----------------------------------
*/

t_sector	*new_sector(void);
t_sector	*new_void_sector(void);
void		add_sector(t_sector **sectors, t_sector *new_sectors);
int			get_nb_sectors(t_sector *sector);

/*
**	---------------------------------- player ----------------------------------
*/

int			move(t_map *map, t_player *player);

/*
**	---------------------------------- map ----------------------------------
*/

void		selected_linedef(t_map_editor *map, t_linedef *lines, Uint32 flags);
void		selected_sector_linedef(t_map_editor *map, t_sector *sectors, Uint32 flags);

void		delete_linedef(t_linedef **lines, Uint32 delete_flags);
void		delete_sector_linedef(t_sector *sectors, Uint32 delete_flags);

SDL_bool 	is_next_to_linedef(t_map_editor *map, t_dot *dot, int radius);
void		map_zoom(t_map_editor *map, double zoom);
void		map_add_line(t_map *map, int n_sector, t_linedef *line);

/*
** =============================================================================
** ================================== PARSING ==================================
** =============================================================================
*/

int 		ft_parse_error(char **tab);
t_sector	*ft_data_storing(int fd, int fd1);
void		ft_find_coord_p1(t_linedef *line, char *tab);
void		ft_find_coord_p2(t_linedef *line, char *tab);
void	    ft_find_type(char *tab, t_linedef *line);
void		ft_find_id(char *id, t_linedef *line);

/*
** =================================================================================
** ================================== PHYSICS ======================================
** =================================================================================
*/

SDL_bool	is_line_horizontal(int y1, int y2, int pitch);
SDL_bool	is_line_vertical(int x1, int x2, int pitch);
SDL_bool	is_in_rect(SDL_Rect rect, t_dot p);
SDL_bool	intersect_line_rect(t_line line, SDL_Rect rect);
SDL_bool 	is_next_point(t_dot dot, t_dot other, int distance);

/*
** =================================================================================
** ================================== EDITOR LOOP ==================================
** =================================================================================
*/

int			editor_loop(t_win *win);
int			editor_event(t_win *win, t_map_editor *map, SDL_bool *loop);
void		resolve_ui_left_release(t_win *win, t_map_editor *map);
void		resolve_ui_left_press(t_win *win, t_map_editor *map);
void		editor_display(t_win *win, const t_map_editor *map);

/*
** ===============================================================================
** ================================== GAME LOOP ==================================
** ===============================================================================
*/

/*
** ================================== Physics ===================================
*/

int			game_loop(t_win *win, t_map *map);
int			raycasting(t_win *win, t_player *player);
int			move(t_map *map, t_player *player);
int			teleportation(t_map *tmap, t_linedef *line1,\
								t_linedef *line2);

/*
** =========================== Math functions ===================================
*/

double		fdist(t_fdot p1, t_fdot p2);
double		dist(t_dot p1, t_dot p2);
double		mag(t_vector vector);
double		fmag(t_fvector vector);
int			sign(double nbr);
double		map(double value, t_dot inter1, t_dot inter2);
double  	modulo(double nbr, double mod);
//void		draw_affine(t_win *win, t_affine function, int xmin, int xmax);
void		draw_affine(t_win *win, t_affine function);

#endif
