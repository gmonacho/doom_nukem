#ifndef PHYSICS_H
# define PHYSICS_H


SDL_bool	is_in_rect(SDL_Rect rect, t_dot p);
SDL_bool	intersect_line_rect(t_line line, SDL_Rect rect);
SDL_bool 	is_next_point(t_dot dot, t_dot other, int distance);

#endif