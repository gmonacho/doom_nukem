#include "loop.h"
#include "display.h"
#include "event.h"
#include "data.h"
#include "ret_error.h"
#include "event.h"
#include "physics.h"

void	draw_line(t_win *win, t_fdot p1, t_fdot p2)
{
	t_vector	delta;
	double		tmp;
	t_fdot		pixel;
	int			i;
	int			n_pixels;

	delta.x = p2.x - p1.x;
	delta.y = p2.y - p1.y;
	if (ft_abs(delta.x) > ft_abs(delta.y))
		tmp = delta.x;
	else
		tmp = delta.y;
	n_pixels = ft_abs(tmp);
	delta.x /= ft_abs(tmp);
	delta.y /= ft_abs(tmp);
	i = -1;
	while (i++ < n_pixels)
	{
		pixel = (t_fdot){.x = p1.x + delta.x * i, .y = p1.y + delta.y * i};
		if (pixel.y < win->height && pixel.y >= 0 &&\
			pixel.x < win->width && pixel.x >= 0)
			SDL_RenderDrawPoint(win->rend, pixel.x, pixel.y);
	}
}
