#include "doom_nukem.h"

void	player_data(char **tab, t_player *player, int i)
{
	while (ft_strrchr(tab[i], '}') == NULL)
	{
		if (ft_strstr(tab[i], "posx ="))
			player->pos_up.x = ft_atoi(ft_strrchr(tab[i], '=') + 1);
		else if (ft_strstr(tab[i], "posy ="))
			player->pos_up.y = ft_atoi(ft_strrchr(tab[i], '=') + 1);
		else if (ft_strstr(tab[i], "posz ="))
			player->pos_up.z = ft_atoi(ft_strrchr(tab[i], '=') + 1);
		else if (ft_strstr(tab[i], "width ="))
			player->width = ft_atoi(ft_strrchr(tab[i], '=') + 1);
		else if (ft_strstr(tab[i], "height ="))
			player->height = ft_atoi(ft_strrchr(tab[i], '=') + 1);
		else if (ft_strstr(tab[i], "dir ="))
			player->dir_init = _2_PI * (double)ft_atoi(
			ft_strrchr(tab[i], '=') + 1) / 360;
		else if (ft_strstr(tab[i], "velocity ="))
			player->const_vel = ft_atoi(ft_strrchr(tab[i], '=') + 1);
		i++;
	}
}
