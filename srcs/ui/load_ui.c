#include "doom_nukem.h"

// des problemes de segfault sur cette partie lors de lectures de fichiers incorrects

// static int		parse_rect(char *line, SDL_Rect *rect)
// {
// 	char		*rect_str;

// 	if (!(rect_str = ft_strstr(line, " : ") + 3))
// 		return (ret_error("parse_rect : invalid line"));
// 	rect->x = ft_atoi(rect_str);
// 	while (!ft_isdigit(*rect_str) && *rect_str)
// 		rect_str++;
// 	rect->y = ft_atoi(rect_str);
// 	while (!ft_isdigit(*rect_str) && *rect_str)
// 		rect_str++;
// 	rect->w = ft_atoi(rect_str);
// 	while (!ft_isdigit(*rect_str) && *rect_str)
// 		rect_str++;
// 	rect->h = ft_atoi(rect_str);
// 	return (1);
// }

static int		parse_float(char *line, double *ratio)
{
	int	i;
	int	tmp;
	int	first_part;
	int	second_part;
	
	first_part = ft_atoi(line);
	i = 0;
	while (ft_isdigit(line[i]) && line[i])
		i++;
	if (line[i] == '.')
	{
		i++;
		second_part = ft_atoi(&line[i]);
		tmp = i;
		while (ft_isdigit(line[i]) && line[i])
			i++;
		*ratio = (double)first_part + second_part / pow(10, i - tmp);
		return (i + 1);
	}
	else
		return (-1);
}

static int		parse_ratio(char *line, t_frect *rect)
{
	int			i;
	int			tmp;
	char		*ratio_str;

	if (!(ratio_str = ft_strstr(line, " : ") + 3))
		return (ret_error("parse_ratio : invalid line"));
	i = 0;
	if ((tmp = parse_float(ft_strstr(line, " : ") + 3, &rect->x)) < 0)
		return (ret_error("parce_ratio : x parse_float failed"));
	i += tmp;
	if ((tmp = parse_float(ft_strstr(line, " : ") + 3 + i, &rect->y)) < 0)
		return (ret_error("parce_ratio : y parse_float failed"));
	i += tmp;
	if ((tmp = parse_float(ft_strstr(line, " : ") + 3 + i, &rect->w)) < 0)
		return (ret_error("parce_ratio : w parse_float failed"));
	i += tmp;
	if ((tmp = parse_float(ft_strstr(line, " : ") + 3 + i, &rect->h)) < 0)
		return (ret_error("parce_ratio : h parse_float failed"));
	return (1);
}

static	int		parse_text(char *line, char **text)
{
	if (!ft_strstr(line, " : "))
		return (ret_error("parse_rect : invalid line"));
	*text = ft_strdup(ft_strstr(line, " : ") + 3);
	return (1);
}

static	int		parse_int(char *line, int *value)
{
	if (!ft_strstr(line, " : "))
		return (ret_error("parse_rect : invalid line"));
	*value = ft_atoi(ft_strstr(line, " : ") + 3);
	return (1);
}

static t_simple_button	*parse_simple_button(char **data_str)
{
	t_simple_button	*simple_button;

	if (!(simple_button = (t_simple_button*)ft_memalloc(sizeof(t_simple_button))))
		return (ret_null_error("parse_simple_button : simple_button allocation failed"));
	if (!parse_text(data_str[2], &simple_button->name))
		return (ret_null_error("parse_simple_button : parse_text failed"));
	return (simple_button);
}

static t_text_entry	*parse_text_entry(char **data_str)
{
	t_text_entry	*text_entry;

	if (!(text_entry = (t_text_entry*)ft_memalloc(sizeof(t_text_entry))))
		return (ret_null_error("parse_text_entry : text_entry allocation failed"));
	if (!parse_text(data_str[2], &text_entry->name))
		return (ret_null_error("parse_text_entry : parse_text failed"));
	if (!parse_int(data_str[3], &text_entry->max_size))
		return (ret_null_error("parse_text_entry : parse_int[1] failed"));
	if (!parse_int(data_str[4], (int*)&text_entry->flags))
		return (ret_null_error("parse_text_entry : parse_int[2] failed"));
	text_entry->variable = NULL;
	return (text_entry);
}

static int				parse_data(char **data_str, void **data, Uint32 flags)
{
	if (flags & BUTTON_SIMPLE)
	{
		if (!(*data = parse_simple_button(data_str)))
			return (ret_error("parse_data : parse_simple_button failed"));
	}
	else if (flags & BUTTON_TEXT_ENTRY)
	{
		if (!(*data = parse_text_entry(data_str)))
			return (ret_error("parse_data : parse_text_entry failed"));
	}
	return (1);
}

static t_button		*parse_button(char **button_str)
{
	t_button	*b;

	if (!(b = (t_button*)ft_memalloc(sizeof(t_button))))
		return (ret_null_error("parse_button : b allocation failed"));
	if (!parse_ratio(button_str[2], &b->ratio))
		return (ret_null_error("parse_button : parse_ratio failed"));
	if (!parse_text(button_str[3], &b->text))
		return (ret_null_error("parse_button : parse_text failed"));
	if (!parse_int(button_str[4], (int*)&b->flags))
		return (ret_null_error("parse_button : parse_int[1] failed"));
	if (!parse_int(button_str[5], (int*)&b->gflags))
		return (ret_null_error("parse_button : parse_int[2] failed"));
	if (!ft_strstr(button_str[6], "data :"))
		return (ret_null_error("parse_button : no data file found"));
	if (!parse_data(&button_str[6], &b->data, b->flags))
		return (ret_null_error("parse_button : parse_data failed"));
	return (b);
}

static t_frame		*parse_frame(t_win *win, char **frame_str)
{
	int			i;
	t_frame		*f;
	t_button	*b;

	if (!(f = (t_frame*)ft_memalloc(sizeof(t_frame))))
		return (ret_null_error("parse_frame : f allocation failed"));
	if (!parse_ratio(frame_str[1], &f->ratio))
		return (ret_null_error("parse_frame : parse_ratio failed"));
	if (!parse_int(frame_str[2], (int*)&f->flags))
		return (ret_null_error("parse_frame : parse_int failed"));
	i = 0;
	while (frame_str[i])
	{
		if (ft_strstr(frame_str[i], "button"))
		{
			if (!(b = parse_button(&frame_str[i])))
				return (ret_null_error("parse_frame : parse_button failed"));
			update_button(win, b, BUTTON_STATE_NONE);
			add_button(&f->buttons, b);
			f->nb_buttons++;
		}
		i++;
	}
	return (f);
}

static char **read_block(int fd)
{
	char	**block;
	char 	**tmp;
	char	*line;
	int		i;
	int		i_bracket;

	block = NULL;
	get_next_line(fd, &line);
	tmp = block;
	i = 0;
	block = ft_2dstrpushback(tmp, i++, line);
	ft_2dstrdel(&tmp);
	ft_strdel(&line);
	i_bracket = 1;
	while (i_bracket != 0 && get_next_line(fd, &line) == 1 )
	{
		tmp = block;
		block = ft_2dstrpushback(tmp, i, line);
		ft_2dstrdel(&tmp);
		if (ft_strchr(line, '{'))
			i_bracket += 1;
		else if (ft_strchr(line, '}'))
			i_bracket -= 1;
		i++;
	}
	return (block);
}

int		load_ui(int fd, t_win *win)
{
	t_frame	*f;
	char	*line;
	char	**block;

	while (get_next_line(fd, &line) && ft_strstr(line, "frame"))
	{
		if (!(block = read_block(fd)))
			return (ret_error("load_ui : read_block failed"));
		if (!(f = parse_frame(win, block)))
			return (ret_error("load_ui : parse_frame failed"));
		f->texture = win->ed_texture.frame_texture;
		add_frame(&win->frames, f);
	}
	return (1);
}	