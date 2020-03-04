#include "doom_nukem.h"
#include "ui_error.h"

void		ed_export(void *ed_export)
{
	int			fd;
	t_mob		*m;
	t_poly		*p;
	t_object	*obj;
	t_export	*export;
	t_map		*map;

	export = (t_export*)ed_export;
	map = (t_map*)export->map;
	printf("path = %s\n", export->path);
	fd = open(export->path,  O_WRONLY | O_APPEND | O_CREAT,
	S_IRUSR	| S_IWUSR | S_IRGRP | S_IROTH);
	if (fd)
	{
		ft_putendl_fd("\n###########", fd);
		ed_write_player(fd, &map->player);
		obj = map->objects;
		while (obj)
		{
			ed_write_item(fd, obj, map->player.pos);
			obj = obj->next;
		}
		m = map->mob;
		while (m)
		{
			ed_write_mob(fd, m);
			m = m->next;
		}
		p = map->polys;
		while (p)
		{
			if (ed_is_real_poly(map, p))
				ed_write_poly(fd, p, &map->player);
			p = p->next;
		}
	}
	else
		ui_ret_error("ed_export", "opening/creating failed", 0);
}


void		ed_package(void *ed_exp)
{
	int			fd;
	t_export	*export;
	t_map		*map;

	export = (t_export*)ed_exp;
	map = (t_map*)export->map;
	fd = open(export->path,  O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR |
	S_IWUSR | S_IRGRP | S_IROTH);
	ft_putendl_fd("#GAMEREADY#", fd);
	ft_putendl_fd("Textures", fd);
	if (!ed_export_textures(fd, map))
	{
		ui_ret_error("ed_package", "failed to export textures", 0);
		return ;
	}
	ft_putendl_fd("Sounds", fd);
	if (!ed_export_sounds(fd))
	{
		ui_ret_error("ed_package", "failed to export textures", 0);
		return ;
	}
	ed_export(ed_exp);
}
