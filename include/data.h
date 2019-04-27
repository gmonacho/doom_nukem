#ifndef DATA_H
# define DATA_H

# include "SDL.h"

/*
**	====================== Data Level ======================
*/

typedef struct	s_linedef
{
	t_fdot		d1;
	t_fdot		d2;
	t_affine	equation;
	int			portal;
	SDL_Texture	*p1p2_texture;
	Uint32		flags;
	struct s_linedef	*next;
}				t_linedef;

typedef struct	s_sector
{
	int			floor_height;
	SDL_Texture	*floor_texture;

	int			ceil_height;
	SDL_Texture	*ceil_texture;

	int			light;
	t_linedef	*lines;
	struct s_sector	*next;
}				t_sector;

typedef struct	s_player
{
	t_fdot		pos;
	int			sector;
	t_vector	vel;
	double		hitbox;
}				t_player;

/*
**	====================== Map ======================
*/

typedef struct	s_map_editor
{
	int			x;
	int			y;
	int			w;
	int			h;
	double		unit;
	t_sector	*sectors;
	int			nb_vectors;
	t_linedef	*lines;
	int			nb_lines;
}				t_map_editor;

typedef struct	s_map
{
	t_sector	*sectors;
	t_player	player;
}				t_map;

int			map_add_line(t_map *map, t_linedef line);
int			map_get_nb_data(void **data);
t_linedef	new_linedef(t_line line, SDL_Texture *p1p2, SDL_Texture *p2p1, Uint32 flags);
void		map_zoom(t_win *win, t_map *map);

double		dist(t_fdot d1, t_fdot d2);

#endif
