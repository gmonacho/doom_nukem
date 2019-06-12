#include "doom_nukem.h"

void	ft_player_data(char **tab, t_player *player)
{
    int i;

    i = 0;
    while (tab[i])
    {   
        if (ft_strstr(tab[i], "Player"))
        {   
            while (ft_strrchr(tab[i], '}') == NULL)
            {   
                if (ft_strstr(tab[i], "posx ="))
                    player->pos.x = ft_atoi(ft_strrchr(tab[i], '=') + 1);
            	else if (ft_strstr(tab[i], "posy ="))
                    player->pos.y = ft_atoi(ft_strrchr(tab[i], '=') + 1);
                else if (ft_strstr(tab[i], "width ="))
                    player->width = ft_atoi(ft_strrchr(tab[i], '=') + 1);
                else if (ft_strstr(tab[i], "height ="))
                    player->height = ft_atoi(ft_strrchr(tab[i], '=') + 1);
                else if (ft_strstr(tab[i], "sector ="))
                    player->numsector = ft_atoi(ft_strrchr(tab[i], '=') + 1);
                else if (ft_strstr(tab[i], "velocity ="))
                    player->const_vel = ft_atoi(ft_strrchr(tab[i], '=') + 1);
                i++;
            }
        }
        i++;
    }
    printf("x = %f\ty = %f\n", player->pos.x, player->pos.y);
	printf("width = %d\theight = %d\n", player->width, player->height);
	printf("Vel = %f\n", player->const_vel);
	printf("Sector = %d\n", player->numsector);
    //printf("\n");
}
