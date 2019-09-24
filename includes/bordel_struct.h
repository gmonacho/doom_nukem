#ifndef BORDEL_STRUCT_H
# define BORDEL_STRUCT_H

# include "SDL.h"

typedef struct		s_dot
{
	int				x;
	int				y;
}					t_dot;

typedef struct		s_fdot
{
	double			x;
	double			y;
}					t_fdot;

typedef struct		s_size
{
	int				w;
	int				h;
}					t_size;

typedef struct		s_line
{
	t_dot			p1;
	t_dot			p2;
}					t_line;

typedef struct		s_fline
{
	t_fdot			p1;
	t_fdot			p2;
}					t_fline;

typedef struct 		s_frect
{
	double			x;
	double			y;
	double			w;
	double			h;
}					t_frect;

typedef struct 		s_circle
{
	int				x;
	int				y;
	int				radius;
}					t_circle;

typedef struct		s_affine
{
	double			a;
	double			b;
	int				isequation;
	double			angle;
}					t_affine;

typedef struct		s_vector
{
	int				x;
	int				y;
}					t_vector;

typedef struct		s_fvector
{
	double			x;
	double			y;
}					t_fvector;

/*
** =====================================================================================
** ================================== EVENT ============================================
** =====================================================================================
*/

enum e_mouse
{
	MOUSE_NONE = 0b0000
};

enum e_mouse_button {MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT};

typedef struct	s_mouse_button
{
	SDL_bool	pressed;
	SDL_bool	pressing;
	SDL_bool	releasing;
}				t_mouse_button;

/*
**	---------------------------------- Mouse ----------------------------------
*/

typedef struct		s_mouse
{
	int				x;
	int				y;
	Uint32			flags;
	t_mouse_button	button[3];
}					t_mouse;

/*
** =======================================================================================
** ================================== DISPLAY ============================================
** =======================================================================================
*/

/*
**	---------------------------------- Interface Management ----------------------------------
*/


typedef enum	e_button_flag
{
	SIMPLE_BUTTON_NONE = 0,
	SIMPLE_BUTTON_NAME = 1
}				t_button_flag;

enum	e_text_entry
{
	TEXT_ENTRY_NONE = 0,
	TEXT_ENTRY_DIGITAL = 1,
	TEXT_ENTRY_ALPHANUM = 2,
	TEXT_ENTRY_TMP = 4,
	TEXT_ENTRY_LINKED = 8,
	TEXT_ENTRY_SECTOR_NAME = 16,
	TEXT_ENTRY_SECTOR_FLOOR = 32,
	TEXT_ENTRY_SECTOR_CEIL = 64
};

typedef enum	e_button_state
{
	BUTTON_STATE_NONE = 0,
	BUTTON_STATE_ON_MOUSE = 1,
	BUTTON_STATE_CLICKED = 2
}				t_button_state;


typedef enum	e_button
{
	BUTTON_NONE = 0,
	BUTTON_DEL_SECTOR = 1,
	BUTTON_TEXT_ENTRY = 2,
	BUTTON_EXPORT = 4,
	BUTTON_GAMELOOP = 8,
	BUTTON_EDITORLOOP = 16,
	BUTTON_ID = 32,
	BUTTON_L_TYPE = 64,
	BUTTON_CLICKED = 128,
	BUTTON_SECTOR_NAME = 256,
	BUTTON_SIMPLE = 512,
	BUTTON_MAP_NAME = 1024,
	BUTTON_MAP_EXPORT = 2048,
	BUTTON_SECTOR_INPUT = 4096,
	BUTTON_LINEDEF_SIDE = 8192,
	BUTTON_ADD_SECTOR = 16384
}				t_button_f;

typedef struct		s_simple_button
{
	char			*name;
	void			*link;
	SDL_bool		clicked;
	t_button_flag	flags;
}					t_simple_button;

typedef struct		s_text_entry
{
	char			*name;
	int				max_size;
	void			*variable;
	Uint8			flags;
}					t_text_entry;

typedef struct		s_checkbox
{
	char			*name;
	SDL_bool		checked;
	void			*variable;
}					t_checkbox;

typedef struct		s_button
{
	t_frect			ratio;
	SDL_Rect		rect;

	SDL_Texture		*texture;
	char			*text;

	Uint32			flags;
	Uint32			gflags;

	void			*data;

	struct s_button	*next;
}					t_button;


enum	e_frame
{
	FRAME_NONE = 0,
	FRAME_SECTORS = 1,
	FRAME_INFO = 2,
	FRAME_HIDE = 4,
	FRAME_L_INFO = 8,
	FRAME_L_TYPE = 16,
	FRAME_MAP = 32,
	FRAME_PLAYER = 64,
	FRAME_PORTAL = 128
};

typedef struct		s_frame
{
	t_frect			ratio;
	SDL_Rect		rect;
	SDL_Texture		*texture;

	t_button		*buttons;
	int				nb_buttons;

	Uint32			flags;

	struct s_frame	*next;
}					t_frame;

/*
**	---------------------------------- hud --------------------------------------------
*/
typedef struct 	s_texHud
{
	SDL_Texture		*tex[15];
	SDL_Texture		*tex_weapon[6];
	SDL_Texture		*tex_reload[5];
	TTF_Font		*police;
}					t_texHud;

/*
**	---------------------------------- Window ----------------------------------
*/

typedef struct		s_ed_texture
{
	SDL_Texture		*frame_texture;
	SDL_Texture		*button;
	SDL_Texture		*clicked_button;
	SDL_Texture		*on_mouse_button;
	SDL_Texture		*digit_tab[10];
}					t_ed_texture;

typedef struct		s_font
{
	TTF_Font		*digital;
	TTF_Font		*ui;
}					t_font;

typedef struct		s_win
{
	SDL_Window*		ptr;
	SDL_Renderer*	rend;

	int				w;
	int				h;
	double			middle_print;

	t_mouse			*mouse;

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
}					t_win;

/*
** ====================================================================================
** ================================== DATA ============================================
** ====================================================================================
*/

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
**	---------------------------------- mob --------------------------------------------
*/
	typedef struct s_mob
	{	
		t_dot			pos;
		int 			live;
		int 			nmob;
		int				sector;
		int 			id;
		char 			*name;
		struct s_mob	*next;
	}					t_mob;
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
	DOOR_CLOSE = 0b0100,
	DOOR_OPEN = 0b00001000
};

typedef enum	e_linedef_side
{
	SIDE_NONE = 0,
	SIDE_RIGHT = 1,
	SIDE_LEFT = 2
}				t_linedef_side;

typedef struct			s_linedef
{
	char				*name;
	t_dot				p1;
	t_dot				p2;
	t_linedef_side		side;
	t_affine			equation;
	double				angle;
	SDL_Surface			*texture;
	Uint32				flags;
	Uint32				gflags;
	int					id;
	struct s_sector		*sector;
	struct s_linedef	*destline;
	struct s_linedef	*next;
}						t_linedef;

/*
**	----------------------------------- Raycasting ---------------------------------------
*/

typedef struct				s_calculs
{
	int						raycast;
	int						column;
	int						nportals;
	double					alpha;
	double					dangle;
	t_affine				ray;
	double					dist;
	double					newdist;
	t_fdot					closest;
	t_linedef				*collision_wall;
}							t_calculs;

/*
**	---------------------------------- sector --------------------------------------------
*/

typedef struct				s_sector
{
	char					*name;

	t_color_picker			color;
	// SDL_Color				color;
	int						floor_height;
	SDL_Texture				*floor_texture;
	int						ceil_height;
	SDL_Texture				*ceil_texture;

	int						height;
	int						light_level;
    t_linedef				*lines;
	// t_dot					center;
	struct	s_sector 		*next;
}							t_sector;

typedef struct				s_textures
{
	SDL_Surface				*elephantride;
	SDL_Surface				*tortue;
}							t_textures;

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
	t_fdot			pos;
	t_dot			dpos;
	double			z;
	char			jump;
	char			shift;
	t_fvector		vel;
	double			const_vel;
	double			dir;
	double			orientation;
	double			fov;
	int				height;
	int				width;
	double			width_2;
	double			width_10;
	double			lenRay;
	int				numsector;
	t_sector		*sector;
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
	double			demipetitaxe;
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
	MAP_NONE = 0b0000,
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
	double		unit;
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

/*
**	---------------------------------- map --------------------------------------------
*/

typedef struct		s_map
{
	t_sector		*sectors;
	t_textures		textures;
	t_player		player;
	t_object		*object;
	t_mob			*mob;
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
