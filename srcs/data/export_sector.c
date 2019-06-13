#include "doom_nukem.h"

static char		*get_line_flag(Uint32 flags)
{
	if (flags & WALL)
		return ("WALL");
	else if (flags & PORTAL)
		return ("PORTAL");
	else if (flags & DOOR_CLOSE)
		return ("DOOR_CLOSE");
	else if (flags & DOOR_OPEN)
		return ("DOOR_OPEN");
	return ("WALL");
}

static void		write_line(int fd, int nb_tab, const char *str)
{
	while (nb_tab--)
	{
		write(fd, "\t", 1);
	}
	ft_putstr_fd(str, fd);
	write(fd, "\n", 1);
}

int				export_sector(t_sector *sector, const char* filename)
{
	int		fd;
	t_linedef	*l;

	if (!(fd = open(ft_strjoin("./maps/sectors/", filename), O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)))
		return(ret_error("failed to open/create file during export_sector"));
	write_line(fd, 0, "Sector");
	write_line(fd, 0, "{");
	write_line(fd, 1, ft_strjoin("name = ", sector->name));
	write_line(fd, 1, ft_strjoin("ceilHeight = ", ft_itoa(sector->ceil_height)));
	write_line(fd, 1, ft_strjoin("floorHeight = ", ft_itoa(sector->floor_height)));
	l = sector->lines;
	while (l)
	{
		write_line(fd, 1, "line");
		write_line(fd, 1, "{");
		write_line(fd, 2, ft_strjoin("dot = (",
							ft_strjoin(ft_itoa(l->p1.x),
							ft_strjoin(", ",
							ft_strjoin(ft_itoa(l->p1.y),
							")")))));
		write_line(fd, 2, ft_strjoin("dot = (",
							ft_strjoin(ft_itoa(l->p2.x),
							ft_strjoin(", ",
							ft_strjoin(ft_itoa(l->p2.y),
							")")))));
		write_line(fd, 2, ft_strjoin("flags = ", get_line_flag(l->flags)));
		write_line(fd, 2, ft_strjoin("id = ", ft_itoa(l->id)));
		write_line(fd, 1, "}");
		l = l->next;
	}
	write_line(fd, 0, "}");
	return (1);
}