#include "doom_nukem.h"
#include "ui_error.h"

int		ed_export(const t_map *map, char *path)
{
	int		fd;
	t_poly	*p;

	if (!(fd = open(path,  O_CREAT | O_WRONLY | O_TRUNC,
						S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)))
		return (ui_ret_error("ed_export", "openning/creating failed", 0));
	ed_write_player(fd, &map->player);
	p = map->polys;
	while (p)
	{
		ed_write_poly(fd, p);
		p = p->next;
	}
	return (1);
}