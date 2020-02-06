#include "doom_nukem.h"

int			count_line(int fp1)
{
	int		nb;
	char	*line;

	nb = 0;
	while (get_next_line(fp1, &line) > 0)
	{
		free(line);
		nb++;
	}
	return (nb);
}

char		**ft_fill_map(int fd, int fp1)
{
	char	*line;
	char	**tab;
	int		i;
	int		nb;

	i = 0;
	nb = count_line(fp1);
	if (!(tab = (char **)malloc(sizeof(char *) * (nb + 1))))
		return (NULL);
	while (get_next_line(fd, &line) > 0)
	{
		tab[i] = ft_strdup(line);
		free(line);
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

char		**fillntesttab(int fd, int fd1)
{
	char **tab;

	tab = NULL;
	tab = ft_fill_map(fd, fd1);
	ft_parse_error(tab);
	return (tab);
}

void		fill_poly(t_poly *poly, t_map *map)
{
	fill_poly_mob(poly, map->mob);
	fill_poly_object(poly, map->object);
	ft_putendl("Fin parsing\n");
}

