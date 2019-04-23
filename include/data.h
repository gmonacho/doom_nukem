#ifndef DATA_H
# define DATA_H

# include "SDL.h"

/*
**	====================== Data Level ======================
*/

typedef struct	s_linedef
{
	t_dot		p1;
	t_dot		p2;
	SDL_Texture	*p1p2_texture;
	SDL_Texture	*p2p1_texture;
	Uint32		flags;
}				t_linedef;

typedef struct	s_sector
{
	int			floor_height;
	SDL_Texture	*floor_texture;

	int			ceil_height;
	SDL_Texture	*ceil_texture;

	int			light_level;
}				t_sector;

/*
**	====================== Map ======================
*/

typedef struct	s_map
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
}				t_map;

int			map_add_line(t_map *map, t_linedef line);
int			map_get_nb_data(void **data);
t_linedef	new_linedef(t_line line, SDL_Texture *p1p2, SDL_Texture *p2p1, Uint32 flags);
void		map_zoom(t_win *win, t_map *map, double zoom);

#endif