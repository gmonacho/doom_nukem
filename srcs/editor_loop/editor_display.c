#include "doom_nukem.h"

static int		draw_id(t_win *win, t_dot pos, int id, double unit)
{
	SDL_Texture		*texture;

	if (!(texture = generate_text(win->rend, win->font.digital, ft_itoa(id), (SDL_Color){200, 0, 200, 255})))
	{
		ft_putendl_fd(SDL_GetError(), 2);
		return (ret_error("draw_id : text generation failed"));
	}	
	if (SDL_RenderCopy(win->rend, texture, NULL, &(SDL_Rect){pos.x, pos.y, ID_SIZE * unit, ID_SIZE * unit}) == -1)
	{
		ft_putendl_fd(SDL_GetError(), 2);
		return (ret_error("draw_id : SDL_RenderCopy failed"));
	}	
	SDL_DestroyTexture(texture);
	return (1);
}

static void			draw_perpendicular(t_win *win, t_linedef l, double unit)
{
	double	a1;
	double	a2;
	int		dx;
	int		dy;
	double	angle;
	t_dot	center;
	int		len;

	len = SIDE_LEN * unit;
	if (l.side != SIDE_NONE)
	{
		if (l.p2.x - l.p1.x != 0)
		{
			if (l.p2.y - l.p1.y == 0)
			{
				dx = 0;
				dy = (l.side == SIDE_RIGHT) ? len : -len;
				if (l.p2.x < l.p1.x)
					dy *= -1;
			}
			else
			{
				a1 = (l.p2.y - l.p1.y) / (double)(l.p2.x - l.p1.x);
				a2 = -1 / a1;
				angle = atan(a2);
				dx = (l.side == SIDE_RIGHT) ? cos(angle) * len : -cos(angle) * len;
				if (l.p2.y > l.p1.y)
					dx *= -1;
				dy = dx * a2;
			}
		}
		else
		{
			dx = (l.side == SIDE_RIGHT) ? len : -len;
			dy = 0;
			if (l.p1.y < l.p2.y)
				dx *= -1;
		}
		center.x = (l.p1.x + l.p2.x) / 2;
		center.y = (l.p1.y + l.p2.y) / 2;
		draw_line(win, center, (t_dot){center.x + dx, center.y + dy});
		draw_id(win, (t_dot){center.x - dx, center.y - dy}, l.id, unit);
	}
}

static void			display_linedefs(t_win *win, const t_map_editor *map, t_sector *s)
{
	t_linedef	*l;
	t_linedef	l_tmp;
	int			color_shift;

	color_shift = (s != map->selected_sector) ? 80 : 0;
	l = s->lines;
	while (l)
	{
		if (l->flags & LINEDEF_SELECTED)
			SDL_SetRenderDrawColor(win->rend, 0, 175 - color_shift, 175 - color_shift, 255);
		else if (l->flags & LINEDEF_MOUSE_POINTED)
			SDL_SetRenderDrawColor(win->rend, 50, 150 - color_shift, 150 - color_shift, 255);
		else
		{
			if (l->gflags & WALL)
				SDL_SetRenderDrawColor(win->rend, 200 - color_shift, 200 - color_shift, 200 - color_shift, 255);
			else if (l->gflags & PORTAL)
				SDL_SetRenderDrawColor(win->rend, 200 - color_shift, 0, 200 - color_shift, 255);
			
		// 	SDL_SetRenderDrawColor(win->rend, s->color.selected_color.r,
		// 										s->color.selected_color.g,
		// 										s->color.selected_color.b,
		// 										s->color.selected_color.a);
		}
		l_tmp.p1 = (t_dot){l->p1.x * map->unit + map->x, l->p1.y * map->unit + map->y};
		l_tmp.p2 = (t_dot){l->p2.x * map->unit + map->x, l->p2.y * map->unit + map->y};
		l_tmp.side = SIDE_RIGHT;
		l_tmp.id = l->id;
		if (is_in_screen(win, l_tmp.p1) || is_in_screen(win, l_tmp.p2))
		{
			draw_line(win, l_tmp.p1, l_tmp.p2);
			if (l->gflags & PORTAL)
				draw_perpendicular(win, l_tmp, map->unit);
		}
		l = l->next;
	}
}

static void			display_buttons(t_win *win, t_frame *f)
{
	t_button	*b;

	b = f->buttons;
	while (b)
	{
		if (b->texture)
			SDL_RenderCopy(win->rend, b->texture, NULL, &b->rect);
		else
			draw_ratio_rect(win, &f->rect, &b->ratio);
		b = b->next;
	}
}

static void			display_frames(t_win *win)
{
	t_frame		*f;

	SDL_SetRenderDrawColor(win->rend, 200, 200, 200, 255);
	f = win->frames;
	while (f)
	{
		if (!(f->flags & FRAME_HIDE))
		{
			if (f->texture)
				SDL_RenderCopy(win->rend, f->texture, NULL, &f->rect);
			else
				draw_ratio_rect(win, &(SDL_Rect){0, 0, win->w, win->h}, &f->ratio);
			display_buttons(win, f);
		}
		f = f->next;
	}
}

static t_sector			*get_player_sector(t_sector *sectors, int i_sector)
{
	t_sector	*s;
	int			i;
	int			i_target;

	s = sectors;
	i = 0;
	i_target = get_nb_sectors(sectors) - i_sector - 1;
	while (s && i < i_target)
	{
		i++;
		s = s->next;
	}
	return (s);
}

void			editor_display(t_win *win, t_map_editor *map)
{
	t_sector	*s;

	SDL_SetRenderDrawColor(win->rend, 100, 100, 100, 255);
	draw_rect(win, (SDL_Rect){map->x, map->y, map->w * map->unit, map->h * map->unit});
	SDL_SetRenderDrawColor(win->rend, 0, 0, 100, 255);
	draw_line(win, (t_dot){map->abscissa.p1.x * map->unit + map->x, map->abscissa.p1.y * map->unit + map->y},
					(t_dot){map->abscissa.p2.x * map->unit + map->x, map->abscissa.p2.y * map->unit + map->y});
	draw_line(win, (t_dot){map->ordinate.p1.x * map->unit + map->x, map->ordinate.p1.y * map->unit + map->y},
					(t_dot){map->ordinate.p2.x * map->unit + map->x, map->ordinate.p2.y * map->unit + map->y});
	SDL_SetRenderDrawColor(win->rend, 255, 100, 100, 255);
	map->player.sector = get_player_sector(map->sectors, map->player.i_sector);
	if (map->player.sector == map->selected_sector)
		draw_circle(win, (t_circle){map->player.dpos.x * map->unit + map->x,
									map->player.dpos.y * map->unit + map->y,
									map->player.width * map->unit});
	if (map->selected_sector)
	{
		s = map->selected_sector;
		display_linedefs(win, map, s);
	}
	s = map->sectors;
	while (s)
	{
		if (s != map->selected_sector)
			display_linedefs(win, map, s);
		s = s->next;
	}
	display_frames(win);
	SDL_SetRenderDrawColor(win->rend, 150, 150, 150, 200);
	if (map->flags & MAP_SELECTING)
		draw_rect(win, map->rect_util);
}