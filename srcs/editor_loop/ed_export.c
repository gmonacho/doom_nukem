#include "doom_nukem.h"
#include "ui_error.h"

void		ed_export(void *ed_export)
{
	t_mob		*m;
	t_poly		*p;
	t_object	*obj;
	t_export	*export;
	t_map		*map;

	export = (t_export*)ed_export;
	map = (t_map*)export->map;
	printf("path = %s\n", export->path);
	if (!export->fd)
		export->fd = open(export->path, O_WRONLY | O_CREAT | O_TRUNC,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (export->fd)
	{
		ft_putendl_fd("\n###########", export->fd);
		ed_write_player(export->fd, &map->player);
		obj = map->objects;
		while (obj)
		{
			ed_write_item(export->fd, obj, map->player.pos);
			obj = obj->next;
		}
		m = map->mob;
		while (m)
		{
			ed_write_mob(export->fd, m);
			m = m->next;
		}
		p = map->polys;
		while (p)
		{
			if (ed_is_real_poly(map, p))
				ed_write_poly(export->fd, p, &map->player);
			p = p->next;
		}
	}
	else
		ui_ret_error("ed_export", "opening/creating failed", 0);
}


void		ed_package(void *ed_exp)
{
	t_export	*export;
	t_map		*map;

	export = (t_export*)ed_exp;
	map = (t_map*)export->map;
	export->fd = open(export->path,  O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR |
	S_IWUSR | S_IRGRP | S_IROTH);
	ft_putendl_fd("#GAMEREADY#", export->fd);
	ft_putendl_fd("Textures", export->fd);
	if (!ed_export_textures(export->fd, map))
	{
		ui_ret_error("ed_package", "failed to export textures", 0);
		return ;
	}
	ft_putendl_fd("Sounds", export->fd);
	if (!ed_export_sounds(export->fd))
	{
		ui_ret_error("ed_package", "failed to export textures", 0);
		return ;
	}
	ed_export(ed_exp);
}
