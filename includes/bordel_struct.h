#ifndef BORDEL_STRUCT_H
# define BORDEL_STRUCT_H

# include "ui_win.h"
# include "SDL.h"

typedef struct		s_fdot_3d
{
	float			x;
	float			y;
	float			z;
}					t_fdot_3d;

typedef struct		s_matrix
{
	float			_00;
	float			_01;
	float			_02;
	float			_10;
	float			_11;
	float			_12;
	float			_20;
	float			_21;
	float			_22;
}					t_matrix;

typedef struct		s_size
{
	int				w;
	int				h;
}					t_size;

typedef struct 		s_circle
{
	int				x;
	int				y;
	int				radius;
}					t_circle;

typedef struct		s_affine
{
	float			a;
	float			b;
	int				isequation;
	float			angle;
}					t_affine;

typedef struct		s_plan
{
	t_fdot_3d		v;
	float			d;
}					t_plan;

typedef struct		s_cartesienne
{
	float			ox;	//Utilise pour les tp de ray intersecteur
	float			oy;
	float			oz;
	float			vx;
	float			vy;
	float			vz;

	struct s_poly	*poly;
	float			dist;
	int				color;
	// t_fdot_3d		collision;
	struct s_cartesienne	*next;
}					t_cartesienne;

/*
** =====================================================================================
** ================================== UI_FUNCTION ======================================
** =====================================================================================
*/

typedef struct	s_arg_menu
{
	int			*loop; // renommer en variable
	int			value;
}				t_arg_menu;

typedef enum	e_editor
{
	ED_NONE = 0,
	ED_MODE_CHANGED = 1,
	ED_SELECTION = 2
}				t_editor_flag;

typedef struct		s_kit_flags
{
	t_editor_flag	*variable;
	int				flags;
}					t_kit_flags;

/*
** =======================================================================================
** ================================== DISPLAY ============================================
** =======================================================================================
*/

/*
**	---------------------------------- hud --------------------------------------------
*/
typedef struct 	s_texHud
{
	SDL_Texture		*tex[14];
	SDL_Texture		*tex_weapon[6];
	SDL_Texture		*tex_reload[5];
	TTF_Font		*police;
}					t_texHud;

/*
**	---------------------------------- main_menu --------------------------------------------
*/

typedef struct s_main_menu
{
	SDL_Texture *text[4];
	SDL_Texture	*textb[4];
	TTF_Font	*police;

}				t_main_menu;

/*
**	---------------------------------- Window ----------------------------------
*/

typedef struct		s_ed_texture
{
	SDL_Texture		*frame_texture;
	SDL_Texture		*button;
	SDL_Texture		*clear;
	SDL_Texture		*clicked_button;
	SDL_Texture		*on_mouse_button;
	SDL_Texture		*digit_tab[10];
}					t_ed_texture;

typedef struct		s_font
{
	TTF_Font		*digital;
	TTF_Font		*ui;
}					t_font;

typedef struct		s_doom_music
{
	Mix_Music		*editor_music;
	Mix_Music		*credit_music;
	Mix_Music		*menu_music;
}					t_doom_music;

typedef enum		e_view
{
	TEXTURE_VIEW = 0b0001,
	WALL_VIEW = 0b0010,
	BOX_VIEW = 0b0100
}					t_view;

typedef struct		s_win
{
	struct s_map	*map;

	SDL_Window*		ptr;
	SDL_Renderer	*rend;
	Uint32			*pixels;
	SDL_Texture		*rend_texture;
	int				view;

	int				w;
	int				h;
	float			w_div_fov;
	float			h_div_fov;

	SDL_Texture		**sectors_texture;
	t_ed_texture	ed_texture;
	SDL_Texture		**sectors_texture_selected;
	SDL_Texture		*text_entry_texture;

	char			*text_entry;

	t_frame			*frames;
	t_frame			*selected_frame;

	t_button		*selected_button;

	t_font			font;
	t_texHud		*texHud;

	t_doom_music	music;
	t_main_menu		*main_menu;

	char			**error_msg;
	int				nb_error_msg;
	
	// libui
	t_winui			*winui;
}					t_win;

/*
** ====================================================================================
** ================================== DATA ============================================
** ====================================================================================
*/

/*
**	---------------------------------- Music --------------------------------------------
*/

typedef struct s_music
{
	Mix_Chunk 	*tmusic[6];
}				t_music;

/*
**	---------------------------------- Timer --------------------------------------------
*/

typedef struct	s_timer
{
	uint32_t		time;
	uint32_t		save;
	int 			index;
}					t_timer;

typedef struct s_timers
{
	t_timer		bullet_cd;
	t_timer		item_cd;
	t_timer		text_cd;
	t_timer		reload_cd;
	t_timer		animation_cd;
	t_timer		shot_cd;
	t_timer		animation_shot_cd;
    t_timer     mouse;
}				t_timers;


/*
**	---------------------------------- png --------------------------------------------
*/

typedef struct	s_png
{
	Uint32		len;
	Uint32		width;
	Uint32		height;
	Uint8		pitch;
	Uint8		color_type;
	Uint8		compression_method;
	Uint8		filter_method;
	Uint8		interlace_method;
	Uint32		*data;
}				t_png;

/*
**	---------------------------------- color_picker --------------------------------------------
*/

typedef struct		s_color_picker
{
	//Uint16			abs_color;
	float			pos;
	SDL_Color		selected_color;
}					t_color_picker;

/*
**	---------------------------------- linedef --------------------------------------------
*/

enum	e_linedef
{
	LINEDEF_NONE = 0b0000,
	LINEDEF_SELECTED = 0b0001,
	LINEDEF_MOUSE_POINTED = 0b0010,
	LINEDEF_MOUSE_NEXT = 0b0100
};

enum	e_glinedef
{
	WALL = 0b0001,
	PORTAL = 0b0010,
	FLOOR = 0b0100,
	CEIL = 0b1000,
	DOOR_OPEN = 0b00010000,
	DOOR_CLOSE = 0b00100000
};

typedef enum	e_linedef_side
{
	SIDE_NONE = 0,
	SIDE_RIGHT = 1,
	SIDE_LEFT = 2
}				t_linedef_side;

typedef struct			s_linedef
{
	char				*name;		//Inutile
	t_dot	    		p1;			//Stocke les donnees du fichier
	t_dot               p2;			//
	t_linedef_side		side;
	
	t_plan				equation;
	t_fdot_3d			origin;
	t_fdot_3d           i;
	t_fdot_3d  			j;
	
	SDL_Surface			*texture;
	Uint32				flags;
	Uint32				gflags;
	int					id;
	
	t_fdot_3d			poly_3d[4];
	t_dot				poly_2d[4];
	t_dot				poly_2d_origin;
	int					poly_w;
	int					poly_h;
	
	struct s_sector		*sector;
	struct s_linedef	*destline;
	struct s_linedef	*next;
}						t_linedef;

/*
**	---------------------------------- sector --------------------------------------------
*/

typedef struct				s_sector
{
	char					*name;
    t_linedef				*lines;
	int						floor_height;
	int						ceil_height;
	int						height;

	// SDL_Color				color;
	// t_plan					floor_equation;
	// SDL_Surface				*floor_texture;
	// t_plan					ceil_equation;
	// SDL_Surface				*ceil_texture;

	t_color_picker			color;
	int						light_level;
	// t_dot					center;
	struct	s_sector 		*next;
}							t_sector;

typedef struct				s_textures
{
	SDL_Surface				*wall_1;
	SDL_Surface				*wall_2;
	SDL_Surface				*floor;
	SDL_Surface				*ceil;
}							t_textures;

/*
**	----------------------------------- Raycasting ---------------------------------------
*/

typedef	struct				s_poly
{
	int						index;
	int						is_slide_ban;
	t_fdot_3d				dots[N_DOTS_POLY];
	t_fdot_3d				dots_rotz_only[N_DOTS_POLY];

	float					dist12;
	float					dist14;
	t_plan					equation;
	t_plan					equation_rotz_only;
	t_fdot_3d				i;
	t_fdot_3d				j;
	t_fdot_3d				i_rotz_only;
	t_fdot_3d				j_rotz_only;
	float					ii;
	float					jj;
	float					ij;
	float					ijij_iijj;

	t_fdot_3d				dots_new[N_DOTS_POLY + 2];	//Un poly ne peut passe que 2 fois sur x
	int						n_dot;
	t_dot					dots_proj[N_DOTS_POLY * 2];
	int						n_proj;	//*2 car une droite ne peut couper que 2 fois le bord de l'ecran
	t_dot					box_x;
	t_dot					box_y;

	SDL_Surface				*texture;
	struct s_poly			*next;
}							t_poly;

typedef struct				s_calculs
{
	int 					dist;
}							t_calculs;

/*
**	---------------------------------- mob --------------------------------------------
*/

typedef struct s_mob
{
	t_poly			*poly;
	t_fdot_3d		pos;
	int 			width;
	int 			width_2;
	int 			height;
	int 			vel;
	int 			velvel;
	int				alive;
	int 			health;
	int				damage;
	int 			type;
	struct s_mob	*next;
	
	int				dir;
	int 			id;
	char 			*name;
}					t_mob;

/*
**	---------------------------------- Inventory --------------------------------------------
*/

typedef struct s_item
{
	SDL_Texture	*text;
	SDL_Rect	*pos;
	int			nb;
}				t_item;

typedef struct s_inventory
{
	t_item		*item[4];
	int			weapon;
	int 		magazine;
	int			ammo;
	int         selected_slot;
}				t_inventory;

/*
**	---------------------------------- player --------------------------------------------
*/

typedef struct		s_player
{
	int				debug;
	int				win_w;
	int				win_h;
	t_fdot			pos;
	t_fdot_3d		pos_up;
	int				on_floor;
	t_sector		*sector;
	t_dot			dpos;
	// float			z;
	// char			jump;
	// char			shift;
	t_fdot			vel;
	float			const_vel;
	// float			dir_up;
	float			dir_init;
	float			fov;
	float			fov_2;
	float			fov_up;
	float			fov_up_2;
	float			ddir;
	float			rot_y;
	int				height;
	float			height_10;
	float			_9_height_10;
	float			_4_height_10;
	int				width;
	float			width_2;
	int				collision_on;

	int				numsector;
	int 			currentHp;
	int 			maxHp;
	int 			currentArmor;
	int				maxArmor;
	t_inventory		*inventory;
	t_timers		timers;
	int				i_sector;
	t_line			l[5];
	int 			*bullet_drop;
	int 			len_bullet;
	float			demipetitaxe;
	int 			damage;

	// t_cartesienne	*rays;
	t_cartesienne	**rays;
	t_matrix		rx;
	t_matrix		rx_inv;
	t_matrix		ry;
	t_matrix		ry_inv;
	t_matrix		rz;
	t_matrix		rz_inv;
}					t_player;

/*
**	---------------------------------- object --------------------------------------------
*/

typedef struct s_object
{
	t_fdot				pos;
	int 				sector;
	int 				id;
	int					id_texture;
	struct s_object		*next;
}			            t_object;

/*
**	---------------------------------- map_editor --------------------------------------------
*/

enum	e_map_editor
{
	MAP_NONE = 0,
	DRAWING_LINE = 1,
	MAP_SELECTING = 2,
	MAP_TEXT_EDITING = 4,
	MAP_MOVING_PLAYER = 8
};

typedef struct	s_map_editor
{
	char		*name;
	int			x;
	int			y;
	int			w;
	int			h;
	float		unit;
	t_sector	*sectors;
	t_sector	*selected_sector;
	SDL_Rect	rect_util;
	Uint32		flags;
	int			nb_lines;
	t_player	player;
	t_line		ordinate;
	t_line		abscissa;
	SDL_bool	abscissa_b;
	SDL_bool	ordinate_b;
}				t_map_editor;

typedef struct	s_editor
{
	t_dot			pos;
	t_dot			size;
	float			unit;
	int				wall_height;
	int				y_min;
	int				y_max;
	t_rect			select_rect;
	t_poly			*selected_poly;
	t_editor_flag	flags;
	t_arg_menu		arg_menu_tab[1];
}				t_editor;

/*
**	---------------------------------- map --------------------------------------------
*/

typedef struct		s_map
{
	t_poly			*polys;
	t_poly			*polys_save;
	// t_sector		*sectors;
	t_textures		textures;
	t_player		player;
	float			gravity;
	t_object		*object;
	t_mob			*mob;
	t_main_menu		main_menu;
	SDL_Event		*event;
	t_music			*music;
	t_editor		editor;
}					t_map;

/*
** =================================================================================
** ================================== EDITOR LOOP ==================================
** =================================================================================
*/

enum	e_sc	// shortcut
{
	SC_NONE = 0,
	SC_DRAW_FREE = SDL_SCANCODE_SPACE
};

#endif
