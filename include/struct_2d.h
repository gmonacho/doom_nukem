#ifndef STRUCT_2D_H
# define STRUCT_2D_H

# include "stdlib.h"

typedef struct		s_dot
{
	int				x;
	int				y;
}					t_dot;

typedef struct		s_line
{
	t_dot			p1;
	t_dot			p2;
}					t_line;

typedef struct 		s_circle
{
	int				x;
	int				y;
	int				radius;
}					t_circle;

typedef struct		s_fdot
{
	double			x;
	double			y;
}					t_fdot;


#endif