#ifndef DATA_H
# define DATA_H

# include "SDL.h"
# include "struct_2d.h"

enum	e_linedef
{
	LINEDEF_SELECTED = 0x0001,
	LINEDEF_MOUSE_POINTED = 0x0010,
	LINEDEF_MOUSE_NEXT = 0x0100,
	LINEDEF_NONE = 0x0000
};

enum	e_map
{
	DRAWING_LINE = 0x0001
};

/*
**	====================== Data Level ======================
*/

typedef struct				s_linedef
{
	t_dot					p1;
	t_dot					p2;
	SDL_Texture				*p1p2_texture;
	SDL_Texture				*p2p1_texture;
	Uint32					flags;
	struct s_linedef		*next;
}							t_linedef;

typedef struct				s_sector
{
	int						floor_height;
	SDL_Texture				*floor_texture;

	int						ceil_height;
	SDL_Texture				*ceil_texture;

	int						light_level;
    t_linedef                  *lines;
	struct s_sector 		*next;
}							t_sector;

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
	t_linedef	*lines;
	SDL_Rect	rect_util;
	Uint32		flags;
}				t_map;

void		add_linedef(t_linedef **lines, t_linedef *new_linedef);
int			map_get_nb_linedef(t_linedef *lines);
t_linedef	*new_linedef(t_line line, SDL_Texture *p1p2, SDL_Texture *p2p1, Uint32 flags);
void		map_zoom(t_map *map, double zoom);
void		selected_linedef(t_map *map, Uint32 flags);
void		delete_linedef(t_linedef **lines, Uint32 delete_flags);
SDL_bool 	is_next_to_linedef(t_linedef **lines, t_dot *dot, int radius);
SDL_bool	is_line_horizontal(int y1, int y2, int pitch);
SDL_bool	is_line_vertical(int x1, int x2, int pitch);
int			map_add_line(t_map *map, t_linedef line);
int 		ft_data_storing(int fd, int fd1);


#endif