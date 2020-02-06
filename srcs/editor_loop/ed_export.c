#include "doom_nukem.h"
#include "ui_error.h"

void		ed_export(void *ed_export)
{
	int			fd;
	t_mob		*m;
	t_poly		*p;
	t_export	*export;
	t_map		*map;

	export = (t_export*)ed_export;
	map = (t_map*)export->map;
	printf("path = %s\n", export->path);
	fd = open(export->path,  O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR
							| S_IWUSR | S_IRGRP | S_IROTH);
	if (fd)
	{
		ed_write_player(fd, &map->player);
		m = map->mob;
		while (m)
		{
			ed_write_mob(fd, m);
			m = m->next;
		}
		p = map->polys;
		while (p)
		{
			ed_write_poly(fd, p);
			p = p->next;
		}
	}
	else
		ui_ret_error("ed_export", "openning/creating failed", 0);
}