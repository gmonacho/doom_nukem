#ifndef DATA_H
# define DATA_H

# include "SDL.h"
# include "struct_2d.h"
 
/*
**	====================== PNG ======================
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

int		parser_png(char *png_file);

/*
**	====================== Data Level ======================
*/

enum	e_linedef
{
	LINEDEF_SELECTED = 0x0001,
	LINEDEF_MOUSE_POINTED = 0x0010,
	LINEDEF_MOUSE_NEXT = 0x0100,
	LINEDEF_NONE = 0x0000
};

typedef struct				s_linedef
{
	t_dot					p1;
	t_dot					p2;
	SDL_Texture				*p1p2_texture;
	SDL_Texture				*p2p1_texture;
	Uint32					flags;
	struct s_linedef		*next;
}							t_linedef;

void		add_linedef(t_linedef **lines, t_linedef *new_linedef);
int			map_get_nb_linedef(t_linedef *lines);
t_linedef	*new_linedef(t_line line, SDL_Texture *p1p2, SDL_Texture *p2p1, Uint32 flags);
SDL_bool	is_line_horizontal(int y1, int y2, int pitch);
SDL_bool	is_line_vertical(int x1, int x2, int pitch);

# define MAX_SECTORS 25

typedef struct				s_sector
{
	char					*name;

	SDL_Color				color;
	int						floor_height;
	SDL_Texture				*floor_texture;

	int						ceil_height;
	SDL_Texture				*ceil_texture;

	int						light_level;
    t_linedef				*lines;
	struct s_sector 		*next;
}							t_sector;


t_sector	*new_sector();
void		add_sector(t_sector **sectors, t_sector *new_sector);
int			get_nb_sectors(t_sector *sector);

/*
**	====================== Map ======================
*/

enum	e_map
{
	DRAWING_LINE = 0x0001,
	MAP_SELECTING = 0x0010
};

typedef struct	s_map
{
	int			x;
	int			y;
	int			w;
	int			h;
	double		unit;
	t_sector	*sectors;
	t_sector	*selected_sector;
	t_linedef	*lines;
	SDL_Rect	rect_util;
	Uint32		flags;
}				t_map;

void		map_zoom(t_map *map, double zoom);
int			map_add_line(t_map *map, t_linedef line);
int			map_get_nb_data(void **data);
//t_linedef	new_linedef(t_line line, SDL_Texture *p1p2, SDL_Texture *p2p1, Uint32 flags);
//void		map_zoom(t_win *win, t_map *map);
void 		ft_data_storing(int fd, int fd1);
void		ft_find_coord_p1(t_sector *sector, char *tab);
void		ft_find_coord_p2(t_sector *sector, char *tab);
void		add_sector(t_sector **sectors, t_sector *new_sectors);
void		add_linedef(t_linedef **linedef, t_linedef *new_linedef);
t_sector	*new_sector(void);
t_linedef	*new_void_linedef(void);
int 		ft_parse_error(char **tab);
void		selected_linedef(t_map *map, Uint32 flags);
void		delete_linedef(t_map *map, Uint32 delete_flags);
SDL_bool 	is_next_to_linedef(t_map *map, t_dot *dot, int radius);

/*
**	====================== Parsing ======================
*/


#endif