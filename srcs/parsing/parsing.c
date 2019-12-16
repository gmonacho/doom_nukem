#include "doom_nukem.h"

// void		ft_fill_coord(t_sector **sector, char **tab, int i)
// {
// 	t_linedef	*line;
// 	int			y;
// 	int			flag;

// 	if (!(line = (t_linedef*)ft_memalloc(sizeof(t_linedef))))
// 	{	
// 		ft_putendl("Error malloc parsing.c l.11\n");
// 		exit(0);
// 	}
// 	y = 0;
// 	flag = 0;
// 	while (!(ft_strchr(tab[i], '}')))
// 	{
// 		if (ft_strstr(tab[i], "dot =") && flag == 0)
// 		{
// 			ft_find_coord_p1(line, tab[i]);
// 			flag++;
// 		}
// 		else if (ft_strstr(tab[i], "dot =") && flag != 0)
// 			ft_find_coord_p2(line, tab[i]);
// 		else if (ft_strstr(tab[i], "flags ="))
// 			ft_find_type(tab[i], line);
// 		else if (ft_strstr(tab[i], "id ="))			//////	Changes : agiordan le gros dep
// 			ft_find_id(tab[i], line);
// 		i++;
// 	}
// 	add_linedef(&((*sector)->lines), line);
// }

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

void find_texture(char *tab, t_poly *poly)
{	
	SDL_Texture *tmp;

	tmp = NULL;
	if (ft_strstr(tab, "texture = Brique.png"))
		poly->texture = IMG_Load("textures/randomPNG/Brick.png");
	else if (ft_strstr(tab, "texture = elephantride.png"))
		poly->texture = IMG_Load("textures/elephantride.png");
	else if (ft_strstr(tab, "texture = plafond.png"))
		poly->texture = IMG_Load("textures/plafond.png");
	else if (ft_strstr(tab, "texture = Pierre.png"))
		poly->texture = IMG_Load("textures/mur_pierre.png");
	else if (ft_strstr(tab, "texture = Papier.png"))
		printf("Papier\n");
	else if (ft_strstr(tab, "texture = Ciseaux.png"))
		printf("Ciseaux\n");
	else if (ft_strstr(tab, "texture = JaiGagne!.png"))
		printf("JaiGagne!\n");
}

void		ft_fill_data(char **tab, t_poly **poly, int i)
{	
	
	int index;

	index = 0;
	add_poly(poly);
	while ((ft_strchr(tab[i], '}') == NULL))
	{	
		if (ft_strstr(tab[i], "dot = "))
		{	
			(*poly)->dots[index].x = ft_atoi(ft_strrchr(tab[i], 'x') + 2);
			(*poly)->dots[index].y = ft_atoi(ft_strrchr(tab[i], 'y') + 2);
			(*poly)->dots[index].z = ft_atoi(ft_strrchr(tab[i], 'z') + 2);
			index++;
		}
		if (ft_strstr(tab[i], "texture ="))
			find_texture(tab[i], *poly);
		i++;
	}
	/*printf("floor_heignt = %d\n", sector->floor_height);
	printf("ceil_height = %d\n", sector->ceil_height);
	printf("name = %s\n", sector->name);
	printf("flags = %u\n", sector->lines->flags);
	printf("p1.x = %d p1.y = %d\n", sector->lines->p1.x, sector->lines->p1.y);
	printf("p2.x = %d p2.y = %d\n", sector->lines->p2.x, sector->lines->p2.y);
	printf("id = %d\n", sector->lines->id);*/
}

t_poly	*ft_data_storing(int fd, int fd1, t_map *map, t_win *win)
{
	char		**tab;
	int			i;
	t_poly		*poly;

	i = -1;
	poly = NULL;								//Modif: agiordan
	// poly = ft_memalloc(sizeof(t_poly));		//Empeche la creation dun mayon vide
	tab = ft_fill_map(fd, fd1);
	ft_parse_error(tab);
	win->texHud = define_texHud(win);
	while (tab[++i])
	{
		if (ft_strstr(tab[i], "Polygon"))
			ft_fill_data(tab, &poly, i);
		else if (ft_strstr(tab[i], "Object"))
			object_data(tab, map->object, i);
		else if (ft_strstr(tab[i], "Player"))
			player_data(tab, &(map->player), i);
		else if (ft_strstr(tab[i], "Mob"))
			fill_mob_data(&(map->mob), tab, i);
	}
	printf("Fin parsing\n\n");
	return (poly);
}
