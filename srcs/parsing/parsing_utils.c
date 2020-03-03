#include "doom_nukem.h"
#include "ui_error.h"

void		fill_poly_object_norm(char *tmp, t_poly *poly_object)
{
	if (!(poly_object->texture = IMG_Load(tmp)))
	{
		ui_ret_error("fill_poly_object", SDL_GetError(), 0);
		exit(0);
		return ;
	}
	if (!(poly_object->texture = SDL_ConvertSurfaceFormat(
		poly_object->texture, SDL_PIXELFORMAT_ARGB8888, 0)))
	{
		ui_ret_error("fill_poly_object", SDL_GetError(), 0);
		exit(0);
		return ;
	}
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

char		**fillntesttab(int fd)
{
	char		**tab;
	int			i;
	static char tmp[13];

	i = 0;
	tmp[12] = '\0';
	if (read(fd, tmp, 12) > 0 && (!ft_strcmp(tmp, "#GAMEREADY#\n")))
	{
		mkdir("textures", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		mkdir("sounds", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (!create_tmp_files(fd, "textures/"))
			ft_putendl("failed to create tmp textures files");
		if (!create_tmp_files(fd, "sounds/"))
			ft_putendl("failed to create tmp sound files");
		// mkdir("TTF", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
	tab = NULL;
	tab = ft_fill_map(fd);
	ft_parse_error(tab);
	return (tab);
}

void		fill_poly(t_poly *poly, t_map *map)
{
	fill_poly_mob(poly, map->mob);
	fill_poly_object(poly, map->objects);
	ft_putendl("Fin parsing\n");
}
