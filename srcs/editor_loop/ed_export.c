#include "doom_nukem.h"
#include "ui_error.h"

int ed_export_ready(int fd, t_map *map)
{
	ft_putendl_fd("#GAMEREADY#", fd);
	ft_putendl_fd("Textures", fd);
	if (!ed_export_textures(fd, map))
		return (0);
	ft_putendl_fd("Sounds", fd);
	if (!ed_export_sounds(fd))
		return (0);
	return (1);
}

void		ed_export(void *ed_export)
{
	int			fd;
	t_mob		*m;
	t_poly		*p;
	t_object	*obj;
	t_export	*export;
	t_map		*map;

	int grdy; // METTRE A 1 POUR EXPORTER AVEC LES .PNG ET AUDIO

	grdy = 0; // TO DO: AJOUTER UNE OPTION/BOUTON 'PACKAGE' SUR L'EDITEUR
	export = (t_export*)ed_export;
	map = (t_map*)export->map;
	printf("path = %s\n", export->path);
	fd = open(export->path,  O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR
							| S_IWUSR | S_IRGRP | S_IROTH);
	if (fd)
	{
		if (grdy && !ed_export_ready(fd, map))
			ui_ret_error("ed_export (gameready)", "opening/creating failed", 0);
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