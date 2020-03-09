#include "doom_nukem.h"
#include "ui_error.h"

int		fill_poly_object_norm(char *tmp, t_poly *poly_object)
{
	if (!(poly_object->texture = IMG_Load(tmp)))
	{
		ui_ret_error("fill_poly_object", SDL_GetError(), 0);
		return (-1);
	}
	if (!(poly_object->texture = SDL_ConvertSurfaceFormat(
		poly_object->texture, SDL_PIXELFORMAT_ARGB8888, 0)))
	{
		ui_ret_error("fill_poly_object", SDL_GetError(), 0);
		return (-1);
	}
	return (0);
}

char		**lst_to_tab(t_list *lst, int height)
{
	int		i;
	char	**tab;
	t_list	*l;

	i = 0;
	l = lst;
	if (!(tab = (char **)malloc(sizeof(char *) * (height + 1))))
		return (NULL);
	tab[height] = NULL;
	while (l && i < height)
	{
		if (!(tab[i] = ft_strdup(l->content)))
			return (NULL);
		i++;
		l = l->next;
	}
	ft_free_list(lst);
	return (tab);
}

char		**ft_fill_map(int fd)
{
	char	*line;
	t_list	*lst;
	int		i;
	int		ret;
	int		grdy;

	i = 0;
	lst = NULL;
	grdy = 0;
	while ((ret = get_next_line(fd, &line)) != 0) //comme ca on ne read qu'une seule fois, c + opti
	{
		printf("%s\n", line);
		if (ret == -1 || (lst = ft_lst_pb(&lst, line)) == NULL)
		{
			ft_free_list(lst);
			return (NULL);
		}
		i++;
	}
	return (lst_to_tab(lst, i));
}

void clear_leaks(t_map *map)
{
	t_poly		*poly_tmp_next;
	t_object	*object_tmp_next;
	t_mob		*mob_tmp_next;

	if (map->polys)
	{
		while (map->polys)
		{
			poly_tmp_next = map->polys->next;
			free(map->polys);
			map->polys = poly_tmp_next;
		}
		map->polys = NULL;
	}
	if (map->mob)
	{
		while (map->mob)
		{
			mob_tmp_next = map->mob->next;
			free(map->mob);
			map->mob = mob_tmp_next;
		}
		map->mob = NULL;
	}
	if (map->objects)
	{
		while (map->objects)
		{
			object_tmp_next = map->objects->next;
			free(map->objects);
			map->objects = object_tmp_next;
		}
		map->objects = NULL;
	}
}

char		**fillntesttab(int fd)
{
	char		**tab;
	int			i;
	static char tmp[13];

	tmp[12] = '\0';
	i = 0;
	if ((read(fd, tmp, 12)) > 0)
	{
		if (!(ft_strcmp(tmp, "#GAMEREADY#\n")))
		{
			mkdir("textures", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			mkdir("sounds", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			if (!create_tmp_files(fd, "textures/"))
				ft_putendl("failed to create tmp textures files");
			if (!create_tmp_files(fd, "sounds/"))
				ft_putendl("failed to create tmp sound files");
			// mkdir("TTF", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
		else if ((ft_strcmp(tmp, "\n###########")) != 0)
		{
			ft_putendl("error map syntax");
			return (NULL);
		}
	} 
	tab = NULL;
	tab = ft_fill_map(fd);
	ft_parse_error(tab);
	return (tab);
}

int		fill_poly(t_poly *poly, t_map *map)
{	
	if (fill_poly_mob(poly, map->mob) == -1 || fill_poly_object(poly, map->objects) == -1)
		return (-1);
	ft_putendl("Fin parsing\n");
	return (0);
}
