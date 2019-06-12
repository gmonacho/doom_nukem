#include "doom_nukem.h"

void		ft_fill_coord(t_sector *sector, char **tab, int i)
{
	t_linedef	*line;
	int			y;
	int			flag;

	if (!(line = (t_linedef *)ft_memalloc(sizeof(t_linedef))))
		exit(0);
	y = 0;
	flag = 0;
	while (!(ft_strchr(tab[i], '}')))
	{
		if (ft_strstr(tab[i], "dot =") && flag == 0)
		{
			ft_find_coord_p1(line, tab[i]);
			flag++;
		}
		else if (ft_strstr(tab[i], "dot =") && flag != 0)
			ft_find_coord_p2(line, tab[i]);
		else if (ft_strstr(tab[i], "flags ="))
			ft_find_type(tab[i], line);
		else if (ft_strstr(tab[i], "id ="))			//////	Changes : agiordan
			ft_find_id(tab[i], line);
		i++;
	}
	add_linedef(&sector->lines, init_linedef(line));
}

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

void		ft_fill_data(char **tab, t_sector *sector, int i)
{	
	add_sector(&sector);
	while ((ft_strchr(tab[i], '}') == NULL || ft_strchr(tab[i - 1], '}') == NULL))
	{
		if (ft_strstr(tab[i], "floorHeight ="))
			sector->floor_height =
			ft_atoi(ft_strrchr(tab[i], '=') + 1);
		if (ft_strstr(tab[i], "ceilHeight ="))
			sector->ceil_height = ft_atoi(ft_strrchr(tab[i], '=') + 1);
		if (ft_strstr(tab[i], "name ="))
			sector->name = ft_strdup(ft_strrchr(tab[i], '=') + 1);
		if (ft_strstr(tab[i], "line"))
			ft_fill_coord(sector, tab, i);
		i++;
	}
	printf("floor_heignt = %d\n", sector->floor_height);
	printf("ceil_height = %d\n", sector->ceil_height);
	printf("name = %s\n", sector->name);
	printf("flags = %u\n", sector->lines->flags);
	printf("p1.x = %d p1.y = %d\n", sector->lines->p1.x, sector->lines->p1.y);
	printf("p2.x = %d p2.y = %d\n", sector->lines->p2.x, sector->lines->p2.y);
	printf("id = %d\n", sector->lines->id);
	printf("\n");
}

t_sector	*ft_data_storing(int fd, int fd1, t_map *map, t_player *player)
{
	char		**tab;
	int			i;
	t_sector	*sector;

	i = -1;
	sector = NULL;
	tab = ft_fill_map(fd, fd1);
	ft_parse_error(tab);
	ft_player_data(tab, player);
	while (tab[++i])
	{
		if (ft_strstr(tab[i], "Sector"))
			ft_fill_data(tab, sector, i);
		else if (ft_strstr(tab[i], "Object"))
			object_data(tab, map->object, i);
	}
	printf("Fin parsing\n\n");
	return (sector);
}


/* t_sector	*ft_data_storing(int fd, int fd1, t_map *map, t_player *player)
{
	char		**tab;
	int			i;
	t_sector	*sector;

	i = -1;
	sector = NULL;
	tab = ft_fill_map(fd, fd1);
	ft_parse_error(tab);
	ft_player_data(tab, player);
	while (tab[++i])
	{
		if (ft_strstr(tab[i], "Sector"))
		{
			add_sector(&sector, new_void_sector());
			while ((ft_strchr(tab[i], '}') == NULL ||
			ft_strchr(tab[i - 1], '}') == NULL))
			{
				ft_fill_data(tab[i], sector);
				if (ft_strstr(tab[i], "line"))
				{
					//add_linedef(&sector->lines, new_void_linedef());
					ft_fill_coord(sector, tab, i);
					printf("floor_heignt = %d\n", sector->floor_height);
					printf("ceil_height = %d\n", sector->ceil_height);
					printf("name = %s\n", sector->name);
					printf("flags = %u\n", sector->lines->flags);
					printf("p1.x = %d p1.y = %d\n", sector->lines->p1.x, sector->lines->p1.y);
					printf("p2.x = %d p2.y = %d\n", sector->lines->p2.x, sector->lines->p2.y);
					printf("id = %d\n", sector->lines->id);
					printf("\n");
				}
				i++;
			}
		}
		else if (ft_strstr(tab[i], "Object"))
			object_data(tab, map->object, i);
	}
	printf("Fin parsing\n\n");
	return (sector);
}*/


