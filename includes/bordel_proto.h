#ifndef BORDEL_PROTO_H
# define BORDEL_PROTO_H

# include "SDL.h"

/*
** =====================================================================================
** ================================== MAIN ============================================
** =====================================================================================
*/

int		init_lines(t_map *map);
int		init_sectors(t_map *map, t_player *player);
void	init_player(t_win *win, t_player *player);
int		init_textures(t_textures *textures);

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

int		    keyboard_state(t_win *win, t_player *player);
int		    key_pressed(Uint32 sdl_keycode);
void 		mouse_state(t_win *win, t_player *player, SDL_Event);

/*
**	---------------------------------- Time ----------------------------------
*/

/*
** ==================== Error Management ==========================
*/

int			ret_num_error(const char *error_msg, int i);
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


void		update_selected_ui(t_win *win);
void		update_ui_rect(t_win *win);

/*
**	---------------------------------- texture ----------------------------------
*/

SDL_Texture		*load_texture(SDL_Renderer *rend, const char *file);

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
void		free_buttons(t_button **buttons);

t_simple_button	*new_simple_button(char *name, SDL_bool clicked);
int     		update_button(t_win *win, t_button *b, t_button_state state);

t_text_entry	*new_text_entry(char *name, int max_size, void *variable, Uint8 flags);
int     		update_text_entry_texture(t_win *win, t_button *button, const char *text);
int				fill_variable(t_win *win, t_map_editor *map, t_button *button, const void *result);


/*
**	---------------------------------- frame ----------------------------------
*/

t_frame		*new_frame(const t_frect ratio, SDL_Texture *texture, Uint32 frame_flags, t_button *buttons);
void		add_frame(t_frame **frames, t_frame *new_frame);
void		add_button_to_frame(t_frame **frame, t_button *button);
void		add_frame_flags(t_frame **frame, Uint32 target_flags, Uint32 added_flags);
void		free_frames(t_frame **frames);
t_frame		*get_frame(t_frame **frames, Uint32 flags);

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

void		print_wall(t_win *win, t_linedef *wall, t_player *player, t_calculs *calculs);
void		fill_portals(t_win *win, t_player *player);
void		draw_line(t_win *win, t_dot p1, t_dot p2);
void		draw_column(t_win *win, int x, int ylow, int yup);
void		draw_rect(t_win *win, SDL_Rect rect);
void		fill_rect(t_win *win, SDL_Rect rect);
void		draw_ratio_rect(t_win *win, const SDL_Rect *rect, const t_frect *ratio);
void		draw_circle(t_win *win, t_circle circle);
void	    draw_color_picker(t_win *win, float picker_position, SDL_Rect rect);
void		draw(t_win *win, t_map *map, t_player *player);

/*
**	---------------------------------- texture ----------------------------------
*/

SDL_Texture		*load_texture(SDL_Renderer *rend, const char *file);
void			hud(t_win *win, t_player *player, t_texHud *texHud);
void    		inventory_hud(t_win *win, t_texHud *texHud, t_player *player);
void			print_weapon(t_win *win, t_player *player, t_texHud *texHud);
void            main_animation(t_map *map, t_win *win, t_texHud *texHud);

/*
**	---------------------------------- inventory ----------------------------------
*/

void   	main_inventory(t_win *win, t_player *player);
void    add_items(t_inventory *inventory, int id);
void    print_items(t_win *win, t_inventory *inventory);
void    print_content_slot(t_win *win, t_player *player, t_texHud *texHud);
void    use_item(t_player *player, int slotSelected);
void    reload_ammo(t_player *player);

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
t_linedef	*new_linedef(t_line line, SDL_Surface *texture, Uint32 flags);
t_linedef	*new_void_linedef(void);
int			get_nb_linedef(t_linedef *lines, Uint32 flags);
void		add_linedef_flags(t_linedef **lines, Uint32 flags);

/*
**	---------------------------------- sector ----------------------------------
*/

//t_sector	*create_sector(void);
void		add_sector(t_sector **sectors);
int			get_nb_sectors(t_sector *sector);

/*
**	---------------------------------- player ----------------------------------
*/

void    	damage_heal(t_player *player, int damage, int armor, int heal);

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
t_sector	*ft_data_storing(int fd, int fd1, t_map *map, t_player *player);
void		ft_find_coord_p1(t_linedef *line, char *tab);
void		ft_find_coord_p2(t_linedef *line, char *tab);
void	    ft_find_type(char *tab, t_linedef *line);
void		ft_find_id(char *id, t_linedef *line);

void		ft_player_data(char **tab, t_player *player);

void		object_data(char **tab, t_object *object, int i);
t_texHud   	*define_texHud(t_win *win);
/*
** =============================================================================
** ================================== EXPORT ===================================
** =============================================================================
*/

void			write_line(int fd, int nb_tab, const char *str);
void			write_sector(t_sector *sector, int fd);
void			write_player(t_player *player, int fd);
int				export_sector(t_player *player, t_sector *sector, const char* filename);
int				export_map(t_map_editor *map, const char *filename);

/*
** =================================================================================
** ================================== PHYSICS ======================================
** =================================================================================
*/

SDL_bool		is_line_horizontal(int y1, int y2, int pitch);
SDL_bool		is_line_vertical(int x1, int x2, int pitch);
SDL_bool		is_in_rect(SDL_Rect rect, t_dot p);
SDL_bool		intersect_line_rect(t_line line, SDL_Rect rect);
SDL_bool 		is_next_point(t_dot dot, t_dot other, int distance);
/*
** =================================================================================
** ================================== MENU LOOPS ==================================
** =================================================================================
*/

/*
** ================================== main_menu ===================================
*/

int			main_menu(t_win *win);

/*
** =================================================================================
** ================================== EDITOR LOOP ==================================
** =================================================================================
*/

void		check_file(t_map_editor *map);
int			editor_loop(t_win *win);
int			editor_event(t_win *win, t_map_editor *map, SDL_bool *loop);
void		resolve_ui_left_release(t_win *win, t_map_editor *map);
int			resolve_ui_left_press(t_win *win, t_map_editor *map);
void		editor_display(t_win *win, t_map_editor *map);
	
/*
** ===============================================================================
** ================================== GAME LOOP ==================================
** ===============================================================================
*/

/*
** ================================== Physics ===================================
*/

int			game_loop(t_win *win, t_map *map);
int			physics(t_win *win, t_map *map, t_player *player);
int			actions(t_win *win, t_map *map, t_linedef *portal, double h);
int			raycasting(t_win *win, t_player *player);
void		begin_ray(t_win *win, t_player *player, t_calculs *calculs);
void		set_new_position(t_fdot *pos, t_linedef *line1, t_linedef *line2, t_sector **sector);
void		set_ray_angle(double *ray_angle, t_linedef *line1, t_linedef *line2);
void		set_ray_equation(t_win *win, t_player *player, t_affine *ray, t_fdot source);
t_linedef	*intersection_ray_wall(t_win *win, t_player *player, t_fdot *source, t_sector *sector, t_calculs *calculs);

/*
** ================================== Time ===================================
*/

int     test_timer(t_timer *timer);
void    start_cooldown(t_timer *timer, Uint32 time);
void    init_cd(t_map *map);
void    reload_cd(t_map *map);

/*
** =========================== Math functions ===================================
*/

// double		dist(t_dot p1, t_dot p2);
double		fdist(t_fdot p1, t_fdot p2);
// double		mag(t_vector vector);
// double		fmag(t_fvector vector);
int			sign(double nbr);
void		normalize(double *angle);

double		prop(double value, t_dot inter1, t_dot inter2);
double  	modulo(double nbr, double mod);
void		draw_affine(t_win *win, t_affine function);
void		draw_ray(t_win *win, t_player *player, t_affine ray);
int			lines_intersection(t_fdot *intersection, t_affine *line1, t_affine *line2);

#endif
