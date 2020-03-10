#include "doom_nukem.h"

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

void		ed_write_mob_cont(int fd, const t_mob *m)
{
	char	*tmp;

	tmp = ft_itoa(m->width);
	ed_write_line(fd, "\twidth", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(m->damage);
	ed_write_line(fd, "\tdamage", tmp);
	ft_strdel(&tmp);
	ed_write_line(fd, "\tdir", "-90");
	tmp = ft_itoa(m->vel);
	ed_write_line(fd, "\tvelocity", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(m->light_coef * 100);
	ed_write_line(fd, "\tlight", tmp);
	ft_strdel(&tmp);
	ed_write_line(fd, "\ttexture", m->texture);
}

void		ed_write_mob(int fd, const t_mob *m)
{
	char	*tmp;

	ft_putendl_fd("Mob", fd);
	ft_putendl_fd("{", fd);
	tmp = ft_itoa(m->pos.x);
	ed_write_line(fd, "\tposx", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(m->pos.y);
	ed_write_line(fd, "\tposy", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(m->pos.z);
	ed_write_line(fd, "\tposz", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(m->height);
	ed_write_line(fd, "\theight", tmp);
	ft_strdel(&tmp);
	ed_write_mob_cont(fd, m);
	ft_putendl_fd("}", fd);
}

void		ed_write_item_cont(int fd, const t_object *obj, t_fdot_3d playerpos)
{
	char	*tmp;

	tmp = ft_itoa(obj->pos_rotz_only.x + playerpos.x);
	ed_write_line(fd, "\tposx", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(obj->pos_rotz_only.y + playerpos.y);
	ed_write_line(fd, "\tposy", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(obj->pos_rotz_only.z + playerpos.z);
	ed_write_line(fd, "\tposz", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(obj->width);
	ed_write_line(fd, "\twidth", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(obj->height);
	ed_write_line(fd, "\theight", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(obj->dir);
	ed_write_line(fd, "\tdir", tmp);
	ft_strdel(&tmp);
	tmp = ft_itoa(obj->light_coef * 100);
	ed_write_line(fd, "\tlight", tmp);
	ft_strdel(&tmp);
	ed_write_line(fd, "\ttexture", obj->texture);
}

void		ed_write_item(int fd, const t_object *obj, t_fdot_3d playerpos)
{
	char	*tmp;

	tmp = NULL;
	ft_putendl_fd("Object", fd);
	ft_putendl_fd("{", fd);
	if (obj->type & HEAL)
		tmp = "HEAL";
	else if (obj->type & ARMOR)
		tmp = "ARMOR";
	else if (obj->type & GRAVITY_INV)
		tmp = "GRAVITY_INV";
	else if (obj->type & BULLET)
		tmp = "BULLET";
	else if (obj->type & BOX)
		tmp = "BOX";
	else if (obj->type & DOOR)
		tmp = "DOOR";
	else if (obj->type & LIGHT)
		tmp = "LIGHT";
	else if (obj->type & TP)
		tmp = "TP";
	ed_write_line(fd, "\ttype", tmp);
	ed_write_item_cont(fd, obj, playerpos);
	ft_putendl_fd("}", fd);
}
