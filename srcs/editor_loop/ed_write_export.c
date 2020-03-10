#include "doom_nukem.h"

static void	ed_write_line(int fd, const char *name, const char *value)
{
	ft_putstr_fd(name, fd);
	ft_putstr_fd(" = ", fd);
	ft_putendl_fd(value, fd);
}

void		ed_write_sphere(int fd, const t_sphere *skybox)
{
	char	*tmp;

	if (skybox)
	{
		ft_putendl_fd("Sphere\n{", fd);
		tmp = ft_itoa(skybox->pos_rotz_only.x);
		ed_write_line(fd, "\tposx", tmp);
		ft_strdel(&tmp);
		tmp = ft_itoa(skybox->pos_rotz_only.y);
		ed_write_line(fd, "\tposy", tmp);
		ft_strdel(&tmp);
		tmp = ft_itoa(skybox->pos_rotz_only.z);
		ed_write_line(fd, "\tposz", tmp);
		ft_strdel(&tmp);
		if (!skybox->radius)
			ed_write_line(fd, "\tradius", "10000");
		else
		{
			tmp = ft_itoa(skybox->radius);
			ed_write_line(fd, "\tradius", tmp);
			ft_strdel(&tmp);
		}
		ed_write_line(fd, "\ttexture", "fractale1.jpg");
		ft_putendl_fd("}", fd);
	}
}

void		ed_write_player_cont(int fd, const t_player *player)
{
	char	*tmp;

	tmp = ft_itoa(player->width);
	ed_write_line(fd, "\twidth", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(player->height);
	ed_write_line(fd, "\theight", tmp);
	ft_strdel(&tmp);
	if (!player->const_vel)
		ed_write_line(fd, "\tvelocity", "2");
	else
	{
		tmp = ft_itoa(player->const_vel);
		ed_write_line(fd, "\tvelocity", tmp);
		ft_strdel(&tmp);
	}
}

void		ed_write_player(int fd, const t_player *player)
{
	char	*tmp;

	if (player)
	{
		ft_putendl_fd("Player", fd);
		ft_putendl_fd("{", fd);
		tmp = ft_itoa(player->pos.x);
		ed_write_line(fd, "\tposx", tmp);
		ft_strdel(&tmp);
		tmp = ft_itoa(player->pos.y);
		ed_write_line(fd, "\tposy", tmp);
		ft_strdel(&tmp);
		tmp = ft_itoa(player->pos.z);
		ed_write_line(fd, "\tposz", tmp);
		ft_strdel(&tmp);
		tmp = ft_itoa(player->dir_init);
		ed_write_line(fd, "\tdir", tmp);
		ft_strdel(&tmp);
		ed_write_player_cont(fd, player);
		ft_putendl_fd("}", fd);
	}
}

static void	ed_write_dot(int fd, const t_fdot_3d *dot, const t_player *player)
{
	char	*tmp;

	ft_putstr_fd("\tdot = x:", fd);
	tmp = ft_itoa(dot->x + player->pos.x);
	ft_putstr_fd(tmp, fd);
	ft_strdel(&tmp);
	ft_putstr_fd(" y:", fd);
	tmp = ft_itoa(dot->y + player->pos.y);
	ft_putstr_fd(tmp, fd);
	ft_strdel(&tmp);
	ft_putstr_fd(" z:", fd);
	tmp = ft_itoa(dot->z + player->pos.z);
	ft_putendl_fd(tmp, fd);
	ft_strdel(&tmp);
}
