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

enum	e_button
{
	BUTTON_NONE = 0b0000,
	BUTTON_COLOR_PICKER = 0b0001,
	BUTTON_TEXT_ENTRY = 0b0010,
	BUTTON_EXPORT = 0b0100,
	BUTTON_GAMELOOP = 0b1000,
	BUTTON_EDITORLOOP = 0b10000,
	BUTTON_ID = 0b100000,
	BUTTON_L_TYPE = 0b1000000,
	BUTTON_CLICKED = 0b10000000
};

typedef struct		s_button
{
	t_frect			ratio;
	SDL_Rect		rect;
	SDL_Texture		*texture;
	char			*text;
	Uint32			flags;
	Uint32			gflags;
	struct s_button	*next;
}					t_button;


enum	e_frame
{
	FRAME_NONE = 0b0000,
	FRAME_SECTORS = 0b0001,
	FRAME_INFO = 0b0010,
	FRAME_HIDE = 0b0100,
	FRAME_L_INFO = 0b1000,
	FRAME_L_TYPE = 0b10000
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
**	---------------------------------- Window ----------------------------------
*/

typedef struct		s_win
{
	SDL_Window*		ptr;
	SDL_Renderer*	rend;

	int				w;
	int				h;
	double			middle_print;

	t_mouse			*mouse;

	SDL_Texture		**sectors_texture;
	SDL_Texture		**sectors_texture_selected;
	SDL_Texture		*text_entry_texture;

	char			*text_entry;

	t_frame			*frames;
	t_frame			*selected_frame;

	t_button		*selected_button;

	TTF_Font		*font;
}					t_win;

/*
**	---------------------------------- hud --------------------------------------------
*/
	typedef struct s_hud
	{
		int 	x1;
		int 	x2;
		int 	y1;
		int 	y2;

	}				t_hud;
/*
** ====================================================================================
** ================================== DATA ============================================
** ====================================================================================
*/

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

typedef struct				s_linedef
{
	char				*name;
	t_dot				p1;
	t_dot				p2;
	t_affine			equation;
	int					isequation;
	double				angle;
	SDL_Texture			*texture;
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
	t_affine				ray;
	double					dangle;
	double					alpha;
	double					dist;
	double					newdist;
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
	t_dot					center;
	struct	s_sector 		*next;
}							t_sector;

/*
**	---------------------------------- player --------------------------------------------
*/

typedef struct		s_player
{
	t_fdot			pos;
	double			z;
	char			jump;
	char			shift;
	t_fvector		vel;
	double			const_vel;
	double			dir;
	double			orientation;
	double			fov;
	int				width;
	int				height;
	double			lenRay;
	int				numsector;
	t_sector		*sector;
	int 			currentHp;
	int 			maxHp;
	int 			currentArmor;
	int				maxArmor;
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
	}			t_object;

/*
**	---------------------------------- map_editor --------------------------------------------
*/

enum	e_map_editor
{
	MAP_NONE = 0b0000,
	DRAWING_LINE = 0b0001,
	MAP_SELECTING = 0b0010,
	MAP_TEXT_EDITING = 0b0100
};

typedef struct	s_map_editor
{
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
}				t_map_editor;

/*
**	---------------------------------- map --------------------------------------------
*/

typedef struct		s_map
{
	t_sector		*sectors;
	t_player		player;
	t_object		*object;
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
