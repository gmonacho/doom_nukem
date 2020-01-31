#include "doom_nukem.h"

static void	ed_write_line(int fd, const char *name, const char *value)
{
	ft_putstr_fd(name, fd);
	ft_putstr_fd(" = ", fd);
	ft_putendl_fd(value, fd);
}

void	ed_write_player(int fd, const t_player *player)
{
	char	*tmp;

	ft_putendl_fd("Player", fd);
	ft_putendl_fd("{", fd);
	tmp = ft_itoa(player->pos_up.x);
	ed_write_line(fd, "posx", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(player->pos_up.y);
	ed_write_line(fd, "posy", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(player->pos_up.z);
	ed_write_line(fd, "posz", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(player->dir_init);
	ed_write_line(fd, "dir", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(player->width);
	ed_write_line(fd, "width", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(player->height);
	ed_write_line(fd, "height", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(player->const_vel);
	ed_write_line(fd, "velocity", tmp);
	ft_strdel(&tmp);
	ft_putendl_fd("}", fd);
}

static void	ed_write_dot(int fd, const t_fdot_3d *dot)
{
	char	*tmp;

	ft_putstr_fd("dot = x:", fd);
	tmp = ft_itoa(dot->x);
	ft_putstr_fd(tmp, fd);
	ft_strdel(&tmp);
	ft_putstr_fd(" y:", fd);
	tmp = ft_itoa(dot->y);
	ft_putstr_fd(tmp, fd);
	ft_strdel(&tmp);
	ft_putstr_fd(" z:", fd);
	tmp = ft_itoa(dot->z);
	ft_putendl_fd(tmp, fd);
	ft_strdel(&tmp);
}

void	ed_write_poly(int fd, const t_poly *poly)
{
	ft_putendl_fd("Polygon", fd);
	ft_putendl_fd("{", fd);
	ed_write_dot(fd, &poly->dots[0]);
	ed_write_dot(fd, &poly->dots[1]);
	ed_write_dot(fd, &poly->dots[2]);
	ed_write_dot(fd, &poly->dots[3]);
	ed_write_line(fd, "texture", "Brique.png");
	ft_putendl_fd("}", fd);
}