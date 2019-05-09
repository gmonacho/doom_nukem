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

typedef struct		s_line
{
	t_fdot			d1;
	t_fdot			d2;
}					t_line;

typedef struct		s_affine
{
	double			a;
	double			b;
}					t_affine;

typedef struct		s_vector
{
	double			x;
	double			y;
}					t_vector;

/*
**	================================= Event ================================
*/

typedef struct	s_button
{
	SDL_bool	pressed;
	SDL_bool	pressing;
	SDL_bool	releasing;
}				t_button;

/*
**	============================== Mouse Managment ========================
*/

typedef struct	s_mouse
{
	int			x;
	int			y;
	Uint32		flags;
	t_button	button[3];
}				t_mouse;

/*
** ================================== Window Management ==================================
*/

typedef struct		s_win
{
	SDL_Window		*ptr;
	int				width;
	int				height;
	SDL_Renderer	*rend;
	t_mouse			*mouse;
}					t_win;

/*
**	================================= Data Level ==========================================
*/

typedef struct		s_linedef
{
	t_fdot			d1;
	t_fdot			d2;
	t_affine		equation;
	double			angle;
	SDL_Texture		*p1p2_texture;
	Uint32			flags;
	int				portal;
	struct s_linedef	*next;
}					t_linedef;

typedef struct	s_sector
{
	int				floor_height;
	SDL_Texture		*floor_texture;

	int				ceil_height;
	SDL_Texture		*ceil_texture;

	int				light;
	t_linedef		*lines;
	struct s_sector	*next;
}					t_sector;

typedef struct		s_player
{
	t_fdot			pos;
	t_vector		vel;
	double			const_vel;
	double			dir;
	double			hitbox;
	int				sector;
}					t_player;

/*
**	============================ Map ============================
*/

typedef struct	s_map_editor
{
	int				x;
	int				y;
	int				w;
	int				h;
	double			unit;
	t_sector		*sectors;
	int				nb_vectors;
	t_linedef		*lines;
	int				nb_lines;
}					t_map_editor;

typedef struct		s_map
{
	t_sector		*sectors;
	t_player		player;
}					t_map;

#endif
